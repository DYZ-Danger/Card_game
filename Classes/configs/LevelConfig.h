/**
 * @file LevelConfig.h
 * @brief 关卡配置数据结构
 * @details 存储关卡的静态配置信息，包括卡牌位置和类型
 */

#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "cocos2d.h"
#include "CardEnums.h"

USING_NS_CC;

/**
 * @brief 单张卡牌的配置信息
 */
struct CardConfig
{
    CardFaceType cardFace;  // 牌面
    CardSuitType cardSuit;  // 花色
    Vec2 position;          // 位置
    
    CardConfig() : cardFace(CardFaceType::NONE), cardSuit(CardSuitType::NONE), position(0, 0) {}
};

/**
 * @brief 关卡配置数据
 */
struct LevelConfig
{
    int levelId;                        // 关卡ID
    std::vector<CardConfig> playfield;  // 主牌区卡牌配置
    std::vector<CardConfig> stack;      // 堆牌区卡牌配置
    
    LevelConfig() : levelId(0) {}
};

#endif // __LEVEL_CONFIG_H__
