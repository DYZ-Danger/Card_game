/**
 * @file LevelConfigLoader.h
 * @brief 关卡配置加载器
 * @details 从JSON文件加载关卡配置
 */

#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "cocos2d.h"
#include "../configs/LevelConfig.h"
#include "json/document.h"

USING_NS_CC;

/**
 * @brief 关卡配置加载器
 * @details 提供静态方法加载关卡配置
 */
class LevelConfigLoader
{
public:
    /**
     * @brief 从JSON文件加载关卡配置
     * @param levelId 关卡ID
     * @return LevelConfig 加载的关卡配置
     */
    static LevelConfig loadLevelConfig(int levelId);
    
    /**
     * @brief 从JSON字符串加载关卡配置
     * @param jsonString JSON字符串
     * @return LevelConfig 加载的关卡配置
     */
    static LevelConfig loadLevelConfigFromString(const std::string& jsonString);
    
private:
    LevelConfigLoader() = default;
    
    /**
     * @brief 解析卡牌JSON对象
     * @param obj JSON对象
     * @return CardConfig 解析的卡牌配置
     */
    static CardConfig _parseCardConfig(const rapidjson::Value& obj);
};

#endif // __LEVEL_CONFIG_LOADER_H__
