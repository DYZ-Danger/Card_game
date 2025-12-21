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
        CCLOG("CardView: Created sprite for card suit=%d face=%d", (int)_cardModel->getSuit(), (int)_cardModel->getFace());
    } else {
        CCLOG("CardView: FAILED to create sprite for card suit=%d face=%d", (int)_cardModel->getSuit(), (int)_cardModel->getFace());
    }
    
    // 设置节点大小以便于触摸检测
    setContentSize(Size(GameConfig::kCardWidth, GameConfig::kCardHeight));
    setAnchorPoint(Vec2(0.5f, 0.5f));
    
    // 初始位置
    Vec2 pos = _cardModel->getPosition();
    setPosition(pos);
    CCLOG("CardView: Set position to (%.1f, %.1f)", pos.x, pos.y);
}

Sprite* CardView::_loadCardTexture()
{
    CCLOG("CardView: Loading texture for suit=%d face=%d", (int)_cardModel->getSuit(), (int)_cardModel->getFace());
    
    // 创建卡牌容器节点
    auto cardContainer = Sprite::create();
    cardContainer->setContentSize(Size(GameConfig::kCardWidth, GameConfig::kCardHeight));
    cardContainer->setAnchorPoint(Vec2(0.5f, 0.5f));
    
    // 1. 加载卡牌底面
    auto cardBase = Sprite::create("card_general.png");
    if (cardBase) {
        cardBase->setPosition(Vec2(GameConfig::kCardWidth * 0.5f, GameConfig::kCardHeight * 0.5f));
        cardBase->setAnchorPoint(Vec2(0.5f, 0.5f));
        // 缩放底面以匹配卡牌尺寸
        float scaleX = GameConfig::kCardWidth / cardBase->getContentSize().width;
        float scaleY = GameConfig::kCardHeight / cardBase->getContentSize().height;
        cardBase->setScale(scaleX, scaleY);
        cardContainer->addChild(cardBase);
        CCLOG("  - Loaded card_general.png");
    } else {
        CCLOG("  - FAILED to load card_general.png");
    }
    
    // 确定花色和点数
    int suitIdx = static_cast<int>(_cardModel->getSuit());
    int faceValue = static_cast<int>(_cardModel->getFace());
    
    // 红色花色：方块(1)和红桃(2)，黑色花色：梅花(0)和黑桃(3)
    bool isRed = (suitIdx == 1 || suitIdx == 2);
    const char* colorStr = isRed ? "red" : "black";
    
    // 花色名称
    const char* suitFiles[] = {"club", "diamond", "heart", "spade"};
    
    // 点数字符串（A=1, J=11, Q=12, K=13）
    std::string faceStr;
    if (faceValue == 1) faceStr = "A";
    else if (faceValue == 11) faceStr = "J";
    else if (faceValue == 12) faceStr = "Q";
    else if (faceValue == 13) faceStr = "K";
    else faceStr = StringUtils::format("%d", faceValue);
    
    // 2. 加载中心大数字
    std::string bigNumberPath = StringUtils::format("number/big_%s_%s.png", colorStr, faceStr.c_str());
    auto bigNumber = Sprite::create(bigNumberPath);
    if (bigNumber) {
        bigNumber->setPosition(Vec2(GameConfig::kCardWidth * 0.5f, GameConfig::kCardHeight * 0.5f));
        bigNumber->setAnchorPoint(Vec2(0.5f, 0.5f));
        cardContainer->addChild(bigNumber);
        CCLOG("  - Loaded %s", bigNumberPath.c_str());
    } else {
        CCLOG("  - FAILED to load %s", bigNumberPath.c_str());
    }
    
    // 3. 加载左上角小数字
    std::string smallNumberPath = StringUtils::format("number/small_%s_%s.png", colorStr, faceStr.c_str());
    auto smallNumber = Sprite::create(smallNumberPath);
    if (smallNumber) {
        smallNumber->setPosition(Vec2(GameConfig::kCardWidth * 0.15f, GameConfig::kCardHeight * 0.85f));
        smallNumber->setAnchorPoint(Vec2(0.5f, 0.5f));
        cardContainer->addChild(smallNumber);
        CCLOG("  - Loaded %s", smallNumberPath.c_str());
    } else {
        CCLOG("  - FAILED to load %s", smallNumberPath.c_str());
    }
    
    // 4. 加载右上角花色
    std::string suitPath = StringUtils::format("suits/%s.png", suitFiles[suitIdx]);
    auto suitSprite = Sprite::create(suitPath);
    if (suitSprite) {
        suitSprite->setPosition(Vec2(GameConfig::kCardWidth * 0.85f, GameConfig::kCardHeight * 0.85f));
        suitSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        cardContainer->addChild(suitSprite);
        CCLOG("  - Loaded %s", suitPath.c_str());
    } else {
        CCLOG("  - FAILED to load %s", suitPath.c_str());
    }
    
    return cardContainer;
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
