/**
 * @file LevelConfigLoader.cpp
 * @brief 关卡配置加载器实现
 */

#include "LevelConfigLoader.h"
#include "json/document.h"

LevelConfig LevelConfigLoader::loadLevelConfig(int levelId)
{
    // 构建配置文件路径
    std::string filePath = StringUtils::format("levels/level_%d.json", levelId);
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filePath);
    
    // 读取文件
    std::string jsonContent = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    return loadLevelConfigFromString(jsonContent);
}

LevelConfig LevelConfigLoader::loadLevelConfigFromString(const std::string& jsonString)
{
    LevelConfig config;
    
    // 解析JSON
    rapidjson::Document document;
    document.Parse(jsonString.c_str());
    
    if (document.HasParseError()) {
        CCLOG("Failed to parse level config JSON");
        return config;
    }
    
    // 解析关卡ID
    if (document.HasMember("levelId") && document["levelId"].IsInt()) {
        config.levelId = document["levelId"].GetInt();
    }
    
    // 解析主牌区卡牌
    if (document.HasMember("Playfield") && document["Playfield"].IsArray()) {
        const rapidjson::Value& playfieldArray = document["Playfield"];
        for (rapidjson::SizeType i = 0; i < playfieldArray.Size(); ++i) {
            CardConfig cardConfig = _parseCardConfig(playfieldArray[i]);
            config.playfield.push_back(cardConfig);
        }
    }
    
    // 解析堆牌区卡牌
    if (document.HasMember("Stack") && document["Stack"].IsArray()) {
        const rapidjson::Value& stackArray = document["Stack"];
        for (rapidjson::SizeType i = 0; i < stackArray.Size(); ++i) {
            CardConfig cardConfig = _parseCardConfig(stackArray[i]);
            config.stack.push_back(cardConfig);
        }
    }
    
    return config;
}

CardConfig LevelConfigLoader::_parseCardConfig(const rapidjson::Value& obj)
{
    CardConfig config;
    
    // 解析花色
    if (obj.HasMember("CardSuit") && obj["CardSuit"].IsInt()) {
        int suit = obj["CardSuit"].GetInt();
        config.cardSuit = static_cast<CardSuitType>(suit);
    }
    
    // 解析牌面
    if (obj.HasMember("CardFace") && obj["CardFace"].IsInt()) {
        int face = obj["CardFace"].GetInt();
        config.cardFace = static_cast<CardFaceType>(face);
    }
    
    // 解析位置
    if (obj.HasMember("Position") && obj["Position"].IsObject()) {
        const rapidjson::Value& posObj = obj["Position"];
        float x = 0, y = 0;
        if (posObj.HasMember("x") && posObj["x"].IsNumber()) {
            x = posObj["x"].GetFloat();
        }
        if (posObj.HasMember("y") && posObj["y"].IsNumber()) {
            y = posObj["y"].GetFloat();
        }
        config.position = Vec2(x, y);
    }
    
    return config;
}
