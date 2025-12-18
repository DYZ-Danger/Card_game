/**
 * @file GameView.cpp
 * @brief 游戏主视图实现
 */

#include "GameView.h"
#include "../configs/GameConfig.h"

GameView* GameView::create(GameModel* gameModel)
{
    GameView* pRet = new GameView();
    if (pRet && pRet->init(gameModel)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

GameView::GameView()
    : _gameModel(nullptr)
    , _playfieldNode(nullptr)
    , _stackNode(nullptr)
{
}

GameView::~GameView()
{
    _cardViews.clear();
}

bool GameView::init(GameModel* gameModel)
{
    if (!Node::init()) {
        return false;
    }
    
    _gameModel = gameModel;
    if (!_gameModel) {
        return false;
    }
    
    setContentSize(Size(GameConfig::kDesignWidth, GameConfig::kDesignHeight));
    
    _initializeLayout();
    _initializeButtons();
    
    // 启用触摸事件
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameView::_onTouchBegan, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void GameView::_initializeLayout()
{
    // 创建主牌区
    _createPlayfieldArea();
    
    // 创建堆牌区
    _createStackArea();
}

void GameView::_createPlayfieldArea()
{
    _playfieldNode = Node::create();
    _playfieldNode->setPosition(Vec2(GameConfig::kDesignWidth * 0.5f, GameConfig::kPlayfieldPosY * 0.5f));
    _playfieldNode->setContentSize(Size(GameConfig::kPlayfieldWidth, GameConfig::kPlayfieldHeight));
    addChild(_playfieldNode);
    
    // 绘制区域背景（用于调试）
    auto drawNode = DrawNode::create();
    drawNode->drawRect(
        Vec2::ZERO,
        Vec2(GameConfig::kPlayfieldWidth, GameConfig::kPlayfieldHeight),
        Color4F(0.8f, 0.8f, 0.8f, 0.3f)
    );
    _playfieldNode->addChild(drawNode);
}

void GameView::_createStackArea()
{
    _stackNode = Node::create();
    _stackNode->setPosition(Vec2(GameConfig::kDesignWidth * 0.5f, GameConfig::kStackAreaPosY * 0.5f));
    _stackNode->setContentSize(Size(GameConfig::kStackAreaWidth, GameConfig::kStackAreaHeight));
    addChild(_stackNode);
    
    // 绘制区域背景（用于调试）
    auto drawNode = DrawNode::create();
    drawNode->drawRect(
        Vec2::ZERO,
        Vec2(GameConfig::kStackAreaWidth, GameConfig::kStackAreaHeight),
        Color4F(0.9f, 0.9f, 0.9f, 0.3f)
    );
    _stackNode->addChild(drawNode);
}

void GameView::_initializeButtons()
{
    // 撤销按钮
    auto undoButton = MenuItemFont::create("Undo", [this](Ref* sender) {
        if (_onUndoClickCallback) {
            _onUndoClickCallback();
        }
    });
    
    auto menu = Menu::create(undoButton, nullptr);
    menu->setPosition(Vec2(100, GameConfig::kDesignHeight - 100));
    addChild(menu);
}

void GameView::addCardView(CardView* cardView, bool isPlayfield)
{
    if (!cardView) return;
    
    Card* card = cardView->getCardModel();
    if (!card) return;
    
    Node* parentNode = isPlayfield ? _playfieldNode : _stackNode;
    if (parentNode) {
        parentNode->addChild(cardView);
        _cardViews[card->getCardId()] = cardView;
    }
}

void GameView::removeCardView(CardView* cardView)
{
    if (!cardView) return;
    
    Card* card = cardView->getCardModel();
    if (!card) return;
    
    _cardViews.erase(card->getCardId());
    cardView->removeFromParent();
}

CardView* GameView::getCardViewById(int cardId) const
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end()) {
        return it->second;
    }
    return nullptr;
}

void GameView::setOnCardClickCallback(const std::function<void(int)>& callback)
{
    _onCardClickCallback = callback;
}

void GameView::setOnUndoClickCallback(const std::function<void()>& callback)
{
    _onUndoClickCallback = callback;
}

bool GameView::_onTouchBegan(Touch* touch, Event* event)
{
    // 这是一个简化的触摸处理，实际应该检测具体点击的卡牌
    auto touchPos = touch->getLocation();
    
    // 检测点击的卡牌
    for (auto& pair : _cardViews) {
        CardView* cardView = pair.second;
        if (cardView && cardView->getBoundingBox().containsPoint(touchPos)) {
            if (_onCardClickCallback) {
                _onCardClickCallback(pair.first);
            }
            return true;
        }
    }
    
    return false;
}
