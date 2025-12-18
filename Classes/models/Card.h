/**
 * @file Card.h
 * @brief 卡牌数据模型
 * @details 表示游戏中一张卡牌的数据和状态
 */

#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"
#include "../configs/CardEnums.h"

USING_NS_CC;

/**
 * @brief 卡牌数据模型
 * @details 存储单张卡牌的属性：花色、牌面、位置、状态等
 */
class Card
{
public:
    /**
     * @brief 创建卡牌
     * @param cardId 卡牌唯一ID
     * @param face 牌面类型
     * @param suit 花色类型
     * @return Card* 卡牌对象指针
     */
    static Card* create(int cardId, CardFaceType face, CardSuitType suit);
    
    // Getter方法
    int getCardId() const { return _cardId; }
    CardFaceType getFace() const { return _face; }
    CardSuitType getSuit() const { return _suit; }
    Vec2 getPosition() const { return _position; }
    bool isVisible() const { return _isVisible; }
    int getStackIndex() const { return _stackIndex; }
    CardAreaType getArea() const { return _area; }
    
    // Setter方法
    void setPosition(const Vec2& pos) { _position = pos; }
    void setVisible(bool visible) { _isVisible = visible; }
    void setStackIndex(int index) { _stackIndex = index; }
    void setArea(CardAreaType area) { _area = area; }
    
    /**
     * @brief 计算与另一张卡牌的点数差
     * @param other 另一张卡牌
     * @return int 点数差的绝对值
     */
    int calculateFaceDifference(Card* other) const;
    
    /**
     * @brief 检查是否能匹配另一张卡牌
     * @param other 另一张卡牌
     * @return bool 是否能匹配（点数差1）
     */
    bool canMatch(Card* other) const;
    
private:
    int _cardId;                    // 卡牌唯一ID
    CardFaceType _face;             // 牌面
    CardSuitType _suit;             // 花色
    Vec2 _position;                 // 当前位置
    bool _isVisible;                // 是否翻开
    int _stackIndex;                // 在堆中的位置（-1表示不在堆中）
    CardAreaType _area;             // 所处区域
    
    Card(int cardId, CardFaceType face, CardSuitType suit);
};

#endif // __CARD_MODEL_H__
