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
    if (_gameModel) {
        delete _gameModel;
        _gameModel = nullptr;
    }
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
    
    // 创建撤销管理器
    _undoManager = UndoManager::create(_gameModel);
    if (!_undoManager) {
        return false;
    }
    
    // 注册撤销执行回调
    _undoManager->setOnApplyUndoCallback([this](const UndoRecord& record) {
        this->_applyUndo(record);
    });
    
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
    int playfieldCount = 0;
    for (auto card : _gameModel->getPlayfieldCards()) {
        CardView* cardView = CardView::create(card);
        if (cardView) {
            _gameView->addCardView(cardView, true);
            playfieldCount++;
        }
    }
    
    // Stack区域的卡牌显示策略：
    // - 左边堆：Stack中除顶部外的卡牌，堆叠显示（用于补充）
    // - 右边单独：顶部卡牌，单独显示（用于匹配）
    auto stackCards = _gameModel->getStackCards();
    if (!stackCards.empty()) {
        _createStackCardViews(stackCards);
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
    
    Card* clickedCard = _gameModel->findCardById(cardId);
    if (!clickedCard) {
        return false;
    }
    
    // 分两种情况处理：Playfield卡牌的匹配 和 Stack卡牌的补牌
    if (clickedCard->getArea() == CardAreaType::PLAYFIELD) {
        return _handlePlayfieldCardClick(cardId, clickedCard);
    } else if (clickedCard->getArea() == CardAreaType::STACK) {
        return _handleStackCardClick(cardId, clickedCard);
    }
    
    return false;
}

bool GameController::_handlePlayfieldCardClick(int cardId, Card* clickedCard)
{
    // 获取Stack右边的质底牌（最后一张）
    auto stackCards = _gameModel->getStackCards();
    if (stackCards.empty()) {
        return false;
    }
    
    Card* rightStackCard = stackCards.back();
    if (!rightStackCard) {
        return false;
    }
    
    // 检查是否能匹配：点数差必须为1
    int faceDiff = clickedCard->calculateFaceDifference(rightStackCard);
    if (faceDiff != 1) {
        return false;
    }
    
    // 记录撤销信息
    UndoRecord undoRecord = _createMatchUndoRecord(cardId, clickedCard, rightStackCard);
    if (_undoManager) {
        _undoManager->recordUndo(undoRecord);
    }
    
    // 执行匹配动画
    _executeMatchAnimation(cardId, clickedCard, rightStackCard);
    
    return true;
}

UndoRecord GameController::_createMatchUndoRecord(int cardId, Card* clickedCard, Card* rightStackCard)
{
    UndoRecord undoRecord;
    undoRecord.operationType = UndoRecord::OperationType::PLAYFIELD_TO_STACK;
    undoRecord.sourceCardId = cardId;
    undoRecord.targetCardId = rightStackCard->getCardId();
    undoRecord.sourcePosition = clickedCard->getPosition();
    undoRecord.targetPosition = rightStackCard->getPosition();
    undoRecord.removedStackCardId = rightStackCard->getCardId();
    
    // 保存撤销前Stack中所有卡牌的ID
    auto currentStackCards = _gameModel->getStackCards();
    for (auto card : currentStackCards) {
        undoRecord.stackCardIds.push_back(card->getCardId());
    }
    
    return undoRecord;
}

void GameController::_executeMatchAnimation(int cardId, Card* clickedCard, Card* rightStackCard)
{
    CardView* clickedCardView = _gameView->getCardViewById(cardId);
    CardView* rightStackCardView = _gameView->getCardViewById(rightStackCard->getCardId());
    
    if (clickedCardView && rightStackCardView) {
        Vec2 targetPos = rightStackCard->getPosition();
        clickedCardView->playMoveAnimation(
            targetPos,
            GameConfig::kCardMoveAnimationDuration,
            [this, clickedCard, rightStackCard]() {
                _updateModelAfterMatch(clickedCard, rightStackCard);
                _refreshStackDisplay();
            }
        );
    }
}

void GameController::_updateModelAfterMatch(Card* clickedCard, Card* rightStackCard)
{
    // 移除Playfield中的卡牌
    _gameModel->removePlayfieldCard(clickedCard);
    
    // 移除Stack中右边的质底牌
    _gameModel->removeStackCard(rightStackCard);
    
    // 从视图中移除Playfield卡牌
    CardView* cvToRemove = _gameView->getCardViewById(clickedCard->getCardId());
    if (cvToRemove) {
        _gameView->removeCardView(cvToRemove);
    }
    
    // 从视图中移除右边的质底牌
    cvToRemove = _gameView->getCardViewById(rightStackCard->getCardId());
    if (cvToRemove) {
        _gameView->removeCardView(cvToRemove);
    }
    
    // 将点击的卡牌添加到Stack中作为新的右边牌
    _gameModel->addStackCard(clickedCard);
}

bool GameController::_handleStackCardClick(int cardId, Card* clickedCard)
{
    // 只有Stack左边堆的卡牌才能被点击进行补牌
    // Stack右边的质底牌不能被点击
    auto stackCards = _gameModel->getStackCards();
    if (stackCards.size() < 2) {
        return false;
    }
    
    // 获取Stack右边的质底牌（最后一张）
    Card* rightStackCard = stackCards.back();
    
    // 检查点击的是否是Stack右边的质底牌（最后一张）
    if (clickedCard == rightStackCard) {
        return false;
    }
    
    // 检查点击的卡牌是否确实在Stack中
    bool isInStack = false;
    for (auto card : stackCards) {
        if (card == clickedCard) {
            isInStack = true;
            break;
        }
    }
    
    if (!isInStack) {
        return false;
    }
    
    // 记录撤销信息
    UndoRecord undoRecord;
    undoRecord.operationType = UndoRecord::OperationType::STACK_SUPPLEMENT;
    undoRecord.sourceCardId = cardId;
    undoRecord.targetCardId = rightStackCard->getCardId();
    undoRecord.sourcePosition = clickedCard->getPosition();
    undoRecord.targetPosition = rightStackCard->getPosition();
    undoRecord.removedStackCardId = rightStackCard->getCardId();
    
    // 保存撤销前Stack中所有卡牌的ID
    for (auto card : stackCards) {
        undoRecord.stackCardIds.push_back(card->getCardId());
    }
    
    if (_undoManager) {
        _undoManager->recordUndo(undoRecord);
    }
    
    // 获取点击卡牌的视图
    CardView* clickedCardView = _gameView->getCardViewById(cardId);
    CardView* rightStackCardView = _gameView->getCardViewById(rightStackCard->getCardId());
    
    if (clickedCardView && rightStackCardView) {
        // 执行动画：将左边堆的卡牌移动到右边
        Vec2 targetPos = rightStackCard->getPosition();
        clickedCardView->playMoveAnimation(
            targetPos,
            GameConfig::kCardMoveAnimationDuration,
            [this, clickedCard, rightStackCard, cardId]() {
                // 从Sta ck中移除右边的质底牌
                _gameModel->removeStackCard(rightStackCard);
                
                // 从视图中移除右边的质底牌
                CardView* cvToRemove = _gameView->getCardViewById(rightStackCard->getCardId());
                if (cvToRemove) {
                    _gameView->removeCardView(cvToRemove);
                }
                
                // 重新显示Stack：左边堆 + 新的右边牌（就是被点击的卡牌）
                _refreshStackDisplay();
            }
        );
    }
    
    return true;
}

void GameController::_refreshStackDisplay()
{
    auto updatedStack = _gameModel->getStackCards();
    
    // 移除所有Stack卡牌的视图
    for (auto card : updatedStack) {
        CardView* cv = _gameView->getCardViewById(card->getCardId());
        if (cv) {
            _gameView->removeCardView(cv);
        }
    }
    
    // 重新创建Stack卡牌视图
    if (!updatedStack.empty()) {
        _createStackCardViews(updatedStack);
    }
}

void GameController::_applyUndo(const UndoRecord& record)
{
    if (record.operationType == UndoRecord::OperationType::PLAYFIELD_TO_STACK) {
        _undoPlayfieldToStack(record);
    } else if (record.operationType == UndoRecord::OperationType::STACK_SUPPLEMENT) {
        _undoStackSupplement(record);
    }
}

void GameController::_undoPlayfieldToStack(const UndoRecord& record)
{
    // 撤销：将Playfield卡牌从Stack右边移回Playfield，并恢复Stack原状态
    
    _removeAllStackViews();
    _gameModel->clearStackCards();
    _restoreStackFromRecord(record);
    _restorePlayfieldCard(record);
    _rebuildStackViews();
}

void GameController::_removeAllStackViews()
{
    auto currentStack = _gameModel->getStackCards();
    for (auto card : currentStack) {
        CardView* cv = _gameView->getCardViewById(card->getCardId());
        if (cv) {
            _gameView->removeCardView(cv);
        }
    }
}

void GameController::_restoreStackFromRecord(const UndoRecord& record)
{
    for (int cardId : record.stackCardIds) {
        Card* card = _gameModel->findCardById(cardId);
        if (card) {
            _gameModel->addStackCard(card);
            card->setArea(CardAreaType::STACK);
        }
    }
}

void GameController::_restorePlayfieldCard(const UndoRecord& record)
{
    Card* playfieldCard = _gameModel->findCardById(record.sourceCardId);
    if (playfieldCard) {
        _gameModel->addPlayfieldCard(playfieldCard);
        playfieldCard->setPosition(record.sourcePosition);
        playfieldCard->setArea(CardAreaType::PLAYFIELD);
        
        CardView* playfieldCardView = CardView::create(playfieldCard);
        if (playfieldCardView) {
            _gameView->addCardView(playfieldCardView, true);
        }
    }
}

void GameController::_rebuildStackViews()
{
    auto restoredStack = _gameModel->getStackCards();
    if (!restoredStack.empty()) {
        _createStackCardViews(restoredStack);
    }
}

void GameController::_createStackCardViews(const std::vector<Card*>& stackCards)
{
    // 左边堆：除了最后一张的所有卡牌
    if (stackCards.size() > 1) {
        Vec2 leftStackPos = Vec2(GameConfig::kStackAreaWidth * 0.25f, GameConfig::kStackAreaHeight / 2.0f);
        
        for (size_t i = 0; i < stackCards.size() - 1; ++i) {
            Card* card = stackCards[i];
            float offsetX = i * 10.0f;
            float offsetY = i * -10.0f;
            Vec2 cardPos = Vec2(leftStackPos.x + offsetX, leftStackPos.y + offsetY);
            
            card->setPosition(cardPos);
            CardView* cardView = CardView::create(card);
            if (cardView) {
                _gameView->addCardView(cardView, false);
            }
        }
    }
    
    // 右边单独：最后一张卡牌（质底牌）
    if (!stackCards.empty()) {
        Card* rightCard = stackCards.back();
        Vec2 rightStackPos = Vec2(GameConfig::kStackAreaWidth * 0.75f, GameConfig::kStackAreaHeight / 2.0f);
        rightCard->setPosition(rightStackPos);
        
        CardView* rightCardView = CardView::create(rightCard);
        if (rightCardView) {
            _gameView->addCardView(rightCardView, false);
        }
    }
}

void GameController::_undoStackSupplement(const UndoRecord& record)
{
    // 撤销：恢复Stack补牌前的状态
    
    _removeAllStackViews();
    _gameModel->clearStackCards();
    _restoreStackFromRecord(record);
    _rebuildStackViews();
}

bool GameController::handleUndo()
{
    if (!_undoManager) {
        return false;
    }
    
    if (!_undoManager->hasUndo()) {
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
