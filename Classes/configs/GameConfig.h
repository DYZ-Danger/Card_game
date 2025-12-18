/**
 * @file GameConfig.h
 * @brief 游戏全局配置常量
 * @details 定义游戏的分辨率、区域尺寸等常量
 */

#ifndef __GAME_CONFIG_H__
#define __GAME_CONFIG_H__

#include "cocos2d.h"

USING_NS_CC;

/**
 * @brief 游戏配置类
 */
class GameConfig
{
public:
    // 设计分辨率
    static constexpr float kDesignWidth = 1080.0f;
    static constexpr float kDesignHeight = 2080.0f;
    
    // 主要区域尺寸
    static constexpr float kPlayfieldWidth = 1080.0f;
    static constexpr float kPlayfieldHeight = 1500.0f;
    
    static constexpr float kStackAreaWidth = 1080.0f;
    static constexpr float kStackAreaHeight = 580.0f;
    
    // 卡牌尺寸
    static constexpr float kCardWidth = 120.0f;
    static constexpr float kCardHeight = 160.0f;
    
    // 动画时长
    static constexpr float kCardMoveAnimationDuration = 0.3f;
    static constexpr float kCardFlipAnimationDuration = 0.2f;
    
    // 区域位置
    static constexpr float kPlayfieldPosY = 1500.0f;    // 主牌区Y坐标
    static constexpr float kStackAreaPosY = 750.0f;     // 堆牌区Y坐标
    
private:
    GameConfig() = default;
};

#endif // __GAME_CONFIG_H__
