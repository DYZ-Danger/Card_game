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
    _playfieldNode->setAnchorPoint(Vec2::ZERO);
    _playfieldNode->setPosition(Vec2(0, GameConfig::kStackAreaHeight));
    _playfieldNode->setContentSize(Size(GameConfig::kPlayfieldWidth, GameConfig::kPlayfieldHeight));
    addChild(_playfieldNode);
    
    // 绘制棕色背景
    auto drawNode = DrawNode::create();
    drawNode->drawSolidRect(
        Vec2::ZERO,
        Vec2(GameConfig::kPlayfieldWidth, GameConfig::kPlayfieldHeight),
        Color4F(0.71f, 0.55f, 0.35f, 1.0f)  // 棕色背景
    );
    _playfieldNode->addChild(drawNode, -1);
}

void GameView::_createStackArea()
{
    _stackNode = Node::create();
    _stackNode->setAnchorPoint(Vec2::ZERO);
    _stackNode->setPosition(Vec2::ZERO);
    _stackNode->setContentSize(Size(GameConfig::kStackAreaWidth, GameConfig::kStackAreaHeight));
    addChild(_stackNode);
    
    // 绘制紫色背景
    auto drawNode = DrawNode::create();
    drawNode->drawSolidRect(
        Vec2::ZERO,
        Vec2(GameConfig::kStackAreaWidth, GameConfig::kStackAreaHeight),
        Color4F(0.62f, 0.25f, 0.62f, 1.0f)  // 紫色背景
    );
    _stackNode->addChild(drawNode, -1);
}

void GameView::_initializeButtons()
{
    // 撤销按钮（显示中文"回退"）
    auto undoLabel = Label::createWithSystemFont("回退", "Arial", 36);
    auto undoButton = MenuItemLabel::create(undoLabel, [this](Ref* sender) {
        if (_onUndoClickCallback) {
            _onUndoClickCallback();
        }
    });
    
    auto menu = Menu::create(undoButton, nullptr);
    // 放在下方Stack区域的右所
    menu->setPosition(Vec2(GameConfig::kDesignWidth - 100, GameConfig::kStackAreaHeight / 2));
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
        
        // 记录卡牌所属的区域
        if (isPlayfield) {
            card->setArea(CardAreaType::PLAYFIELD);
        } else {
            card->setArea(CardAreaType::STACK);
        }
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
    // 获取触摸的世界坐标
    auto touchPos = touch->getLocation();
    
    // 检测点击的卡牌 - 从后往前遍历（后添加的卡牌在上面）
    // 反向遍历以检测顶层卡牌
    for (auto it = _cardViews.rbegin(); it != _cardViews.rend(); ++it) {
        CardView* cardView = it->second;
        if (cardView) {
            // 将世界坐标转换为卡牌的本地坐标
            Vec2 localPos = cardView->convertToNodeSpace(touchPos);
            Rect boundingBox = Rect(
                -cardView->getContentSize().width * 0.5f,
                -cardView->getContentSize().height * 0.5f,
                cardView->getContentSize().width,
                cardView->getContentSize().height
            );
            
            if (boundingBox.containsPoint(localPos)) {
                CCLOG("Touch detected on card %d at pos (%.1f, %.1f)", it->first, touchPos.x, touchPos.y);
                if (_onCardClickCallback) {
                    _onCardClickCallback(it->first);
                }
                return true;
            }
        }
    }
    
    return false;
}
