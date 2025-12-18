/**
 * @file CardView.cpp
 * @brief 卡牌视图实现
 */

#include "CardView.h"
#include "../configs/GameConfig.h"

CardView* CardView::create(Card* cardModel)
{
    CardView* pRet = new CardView();
    if (pRet && pRet->init(cardModel)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

CardView::CardView()
    : _cardModel(nullptr)
    , _cardSprite(nullptr)
{
}

CardView::~CardView()
{
}

bool CardView::init(Card* cardModel)
{
    if (!Node::init()) {
        return false;
    }
    
    _cardModel = cardModel;
    if (!_cardModel) {
        return false;
    }
    
    _initializeCardVisuals();
    
    return true;
}

void CardView::_initializeCardVisuals()
{
    // 加载卡牌纹理
    _cardSprite = _loadCardTexture();
    if (_cardSprite) {
        _cardSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        addChild(_cardSprite);
    }
    
    // 设置节点大小以便于触摸检测
    setContentSize(Size(GameConfig::kCardWidth, GameConfig::kCardHeight));
    setAnchorPoint(Vec2(0.5f, 0.5f));
    
    // 初始位置
    setPosition(_cardModel->getPosition());
}

Sprite* CardView::_loadCardTexture()
{
    // 根据卡牌模型数据加载相应的纹理
    // 这里先创建一个默认的矩形作为占位符，实际项目中应该根据卡牌面值和花色加载对应的图片
    
    // 临时方案：使用颜色块作为卡牌显示
    auto sprite = Sprite::create();
    
    // 创建一个简单的渲染纹理作为卡牌显示
    DrawNode* drawNode = DrawNode::create();
    drawNode->drawRect(Vec2::ZERO, Vec2(GameConfig::kCardWidth, GameConfig::kCardHeight), 
                      Color4F(1, 1, 1, 1));
    
    return sprite ? sprite : Sprite::create();
}

void CardView::playMoveAnimation(const Vec2& targetPos, float duration, const std::function<void()>& callback)
{
    if (!_cardModel) return;
    
    auto moveAction = MoveTo::create(duration, targetPos);
    
    if (callback) {
        auto sequence = Sequence::create(moveAction, CallFunc::create(callback), nullptr);
        runAction(sequence);
    } else {
        runAction(moveAction);
    }
    
    _cardModel->setPosition(targetPos);
}

void CardView::playFlipAnimation(bool isFaceUp, float duration)
{
    // 简单的翻牌动画：缩放X轴
    auto scaleDown = ScaleTo::create(duration * 0.5f, 0, 1);
    auto scaleUp = ScaleTo::create(duration * 0.5f, 1, 1);
    auto sequence = Sequence::create(scaleDown, scaleUp, nullptr);
    
    runAction(sequence);
    
    _cardModel->setVisible(isFaceUp);
}

void CardView::setPositionImmediate(const Vec2& pos)
{
    setPosition(pos);
    if (_cardModel) {
        _cardModel->setPosition(pos);
    }
}

void CardView::setVisibleImmediate(bool visible)
{
    setVisible(visible);
    if (_cardModel) {
        _cardModel->setVisible(visible);
    }
}

void CardView::updateDisplay()
{
    if (!_cardModel) return;
    
    setPosition(_cardModel->getPosition());
    setVisible(_cardModel->isVisible());
}
