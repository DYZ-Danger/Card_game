/**
 * @file GameModelGenerator.h
 * @brief 游戏模型生成服务
 * @details 将静态关卡配置转换为动态运行时游戏模型
 */

#ifndef __GAME_MODEL_GENERATOR_H__
#define __GAME_MODEL_GENERATOR_H__

#include "cocos2d.h"
#include "../configs/LevelConfig.h"
#include "../models/GameModel.h"

USING_NS_CC;

/**
 * @brief 游戏模型生成器服务
 * @details 无状态的服务类，根据关卡配置生成游戏运行时模型
 */
class GameModelGenerator
{
public:
    /**
     * @brief 根据关卡配置生成游戏模型
     * @param levelConfig 关卡配置
     * @return GameModel* 生成的游戏模型
     */
    static GameModel* generateGameModel(const LevelConfig& levelConfig);
    
private:
    GameModelGenerator() = default;
    
    /**
     * @brief 创建单张卡牌
     * @param cardConfig 卡牌配置
     * @param cardId 卡牌ID
     * @return Card* 创建的卡牌
     */
    static Card* _createCardFromConfig(const CardConfig& cardConfig, int& cardId);
};

#endif // __GAME_MODEL_GENERATOR_H__
