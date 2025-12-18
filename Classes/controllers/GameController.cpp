/**
 * @file GameController.cpp
 * @brief 游戏主控制器实现
 */

#include "GameController.h"
#include "../services/GameModelGenerator.h"
#include "../views/CardView.h"
#include "../configs/GameConfig.h"

GameController* GameController::create()
{
    GameController* controller = new GameController();
    if (controller && controller->_initialize()) {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

GameController::GameController()
    : _gameModel(nullptr)
    , _gameView(nullptr)
    , _undoManager(nullptr)
    , _selectedCardId(-1)
    , _currentGameState(GameStateType::IDLE)
{
}

GameController::~GameController()
{
    CC_SAFE_RELEASE(_gameModel);
    CC_SAFE_RELEASE(_gameView);
    if (_undoManager) {
        delete _undoManager;
        _undoManager = nullptr;
    }
}

bool GameController::_initialize()
{
    return true;
}

bool GameController::initWithLevelConfig(const LevelConfig& levelConfig)
{
    if (!_initializeModel(levelConfig)) {
        return false;
    }
    
    if (!_initializeView()) {
        return false;
    }
    
    _bindCallbacks();
    
    return true;
}

bool GameController::_initializeModel(const LevelConfig& levelConfig)
{
    // 使用服务生成游戏模型
    _gameModel = GameModelGenerator::generateGameModel(levelConfig);
    if (!_gameModel) {
        return false;
    }
    
    _gameModel->retain();
    
    // 创建撤销管理器
    _undoManager = UndoManager::create(_gameModel);
    if (!_undoManager) {
        return false;
    }
    
    return true;
}

bool GameController::_initializeView()
{
    if (!_gameModel) {
        return false;
    }
    
    // 创建游戏视图
    _gameView = GameView::create(_gameModel);
    if (!_gameView) {
        return false;
    }
    
    _gameView->retain();
    
    // 为每张卡牌创建视图
    for (auto card : _gameModel->getPlayfieldCards()) {
        CardView* cardView = CardView::create(card);
        if (cardView) {
            _gameView->addCardView(cardView, true);
        }
    }
    
    for (auto card : _gameModel->getStackCards()) {
        CardView* cardView = CardView::create(card);
        if (cardView) {
            _gameView->addCardView(cardView, false);
        }
    }
    
    return true;
}

void GameController::_bindCallbacks()
{
    if (!_gameView) return;
    
    // 绑定卡牌点击回调
    _gameView->setOnCardClickCallback([this](int cardId) {
        this->handleCardClick(cardId);
    });
    
    // 绑定撤销点击回调
    _gameView->setOnUndoClickCallback([this]() {
        this->handleUndo();
    });
    
    // 绑定撤销完成回调
    if (_undoManager) {
        _undoManager->setOnUndoCompleteCallback([this](const UndoRecord& record) {
            // 根据撤销记录更新视图
            if (record.sourceCardId >= 0) {
                CardView* cardView = _gameView->getCardViewById(record.sourceCardId);
                if (cardView) {
                    // 播放反向动画：从目标位置回到源位置
                    cardView->playMoveAnimation(record.sourcePosition, GameConfig::kCardMoveAnimationDuration);
                }
            }
        });
    }
}

bool GameController::handleCardClick(int cardId)
{
    if (!_gameModel || !_gameView) {
        return false;
    }
    
    // 获取点击的卡牌
    Card* clickedCard = _gameModel->findCardById(cardId);
    if (!clickedCard) {
        return false;
    }
    
    // 获取堆顶卡牌
    Card* topStackCard = _gameModel->getTopStackCard();
    if (!topStackCard) {
        return false;
    }
    
    // 检查是否能匹配
    if (!clickedCard->canMatch(topStackCard)) {
        CCLOG("卡牌不匹配：%d vs %d", (int)clickedCard->getFace(), (int)topStackCard->getFace());
        return false;
    }
    
    // 记录撤销信息
    UndoRecord undoRecord;
    undoRecord.operationType = UndoRecord::OperationType::CARD_MOVE;
    undoRecord.sourceCardId = cardId;
    undoRecord.targetCardId = topStackCard->getCardId();
    undoRecord.sourcePosition = clickedCard->getPosition();
    undoRecord.targetPosition = topStackCard->getPosition();
    
    if (_undoManager) {
        _undoManager->recordUndo(undoRecord);
    }
    
    // 获取视图
    CardView* clickedCardView = _gameView->getCardViewById(cardId);
    CardView* topStackCardView = _gameView->getCardViewById(topStackCard->getCardId());
    
    if (clickedCardView && topStackCardView) {
        // 执行动画：将点击的卡牌移动到堆顶
        clickedCardView->playMoveAnimation(
            topStackCard->getPosition(),
            GameConfig::kCardMoveAnimationDuration,
            [this, clickedCard, topStackCard]() {
                // 动画完成后更新模型和视图
                _gameModel->removePlayfieldCard(clickedCard);
                _gameModel->removeStackCard(topStackCard);
                
                // 从主牌区移除卡牌视图
                CardView* cvToRemove = _gameView->getCardViewById(clickedCard->getCardId());
                if (cvToRemove) {
                    _gameView->removeCardView(cvToRemove);
                }
                
                cvToRemove = _gameView->getCardViewById(topStackCard->getCardId());
                if (cvToRemove) {
                    _gameView->removeCardView(cvToRemove);
                }
            }
        );
    }
    
    return true;
}

bool GameController::handleUndo()
{
    if (!_undoManager) {
        return false;
    }
    
    return _undoManager->executeUndo();
}

void GameController::startGame()
{
    if (_gameModel) {
        _gameModel->setGameState(GameStateType::PLAYING);
        _currentGameState = GameStateType::PLAYING;
    }
}

void GameController::stopGame()
{
    if (_gameModel) {
        _gameModel->setGameState(GameStateType::PAUSED);
        _currentGameState = GameStateType::PAUSED;
    }
}

void GameController::restartGame()
{
    // 清空撤销历史
    if (_undoManager) {
        _undoManager->clearAll();
    }
    
    // 可以在这里添加重启逻辑
}
