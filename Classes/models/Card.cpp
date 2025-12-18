/**
 * @file Card.cpp
 * @brief 卡牌数据模型实现
 */

#include "Card.h"

Card* Card::create(int cardId, CardFaceType face, CardSuitType suit)
{
    return new Card(cardId, face, suit);
}

Card::Card(int cardId, CardFaceType face, CardSuitType suit)
    : _cardId(cardId)
    , _face(face)
    , _suit(suit)
    , _position(Vec2::ZERO)
    , _isVisible(true)
    , _stackIndex(-1)
    , _area(CardAreaType::NONE)
{
}

int Card::calculateFaceDifference(Card* other) const
{
    if (!other) return -1;
    
    int faceValue1 = static_cast<int>(_face);
    int faceValue2 = static_cast<int>(other->_face);
    
    return std::abs(faceValue1 - faceValue2);
}

bool Card::canMatch(Card* other) const
{
    if (!other) return false;
    
    // 点数差必须为1才能匹配
    return calculateFaceDifference(other) == 1;
}
