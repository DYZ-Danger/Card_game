/**
 * @file CardView.h
 * @brief 卡牌视图
 * @details 负责单张卡牌的UI显示和动画
 */

#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "../models/Card.h"

USING_NS_CC;

/**
 * @brief 卡牌视图类
 * @details 负责卡牌的UI显示、动画等视觉相关的操作
 */
class CardView : public Node
{
public:
    /**
     * @brief 创建卡牌视图
     * @param cardModel 卡牌数据模型
     * @return CardView* 卡牌视图指针
     */
    static CardView* create(Card* cardModel);
    
    /**
     * @brief 获取关联的卡牌模型
     * @return Card* 卡牌模型指针
     */
    Card* getCardModel() const { return _cardModel; }
    
    /**
     * @brief 执行移动动画
     * @param targetPos 目标位置
     * @param duration 动画时长
     * @param callback 完成回调
     */
    void playMoveAnimation(const Vec2& targetPos, float duration, const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 执行翻牌动画
     * @param isFaceUp 是否翻正面
     * @param duration 动画时长
     */
    void playFlipAnimation(bool isFaceUp, float duration);
    
    /**
     * @brief 立即设置位置（不动画）
     * @param pos 位置
     */
    void setPositionImmediate(const Vec2& pos);
    
    /**
     * @brief 立即设置可见性（不动画）
     * @param visible 是否可见
     */
    void setVisibleImmediate(bool visible);
    
    /**
     * @brief 更新视图显示
     */
    void updateDisplay();
    
    /**
     * @brief 获取卡牌节点（内部Sprite）
     * @return Sprite* 卡牌精灵
     */
    Sprite* getCardSprite() const { return _cardSprite; }
    
protected:
    CardView();
    virtual ~CardView();
    
    virtual bool init(Card* cardModel);
    
    /**
     * @brief 初始化卡牌视觉
     */
    void _initializeCardVisuals();
    
    /**
     * @brief 加载卡牌纹理
     */
    Sprite* _loadCardTexture();
    
private:
    Card* _cardModel;               // 关联的数据模型
    Sprite* _cardSprite;            // 卡牌精灵
};

#endif // __CARD_VIEW_H__
