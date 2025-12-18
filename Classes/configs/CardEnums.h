/**
 * @file CardEnums.h
 * @brief 卡牌相关枚举定义
 * @details 定义卡牌花色、牌面等常量枚举
 */

#ifndef __CARD_ENUMS_H__
#define __CARD_ENUMS_H__

/**
 * @brief 花色类型枚举
 */
enum class CardSuitType
{
    CLUBS = 0,      // 梅花
    DIAMONDS = 1,   // 方块
    HEARTS = 2,     // 红桃
    SPADES = 3,     // 黑桃
    NONE = -1
};

/**
 * @brief 牌面类型枚举
 */
enum class CardFaceType
{
    ACE = 1,        // A
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    JACK = 11,      // J
    QUEEN = 12,     // Q
    KING = 13,      // K
    NONE = 0
};

/**
 * @brief 卡牌所处区域类型
 */
enum class CardAreaType
{
    PLAYFIELD = 0,  // 主牌区
    STACK = 1,      // 堆牌区（底牌+备用牌）
    NONE = -1
};

/**
 * @brief 游戏状态
 */
enum class GameStateType
{
    IDLE = 0,           // 空闲
    PLAYING = 1,        // 正在游戏
    GAME_OVER = 2,      // 游戏结束
    PAUSED = 3           // 暂停
};

#endif // __CARD_ENUMS_H__
