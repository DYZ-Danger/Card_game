/**
 * @file GameModelGenerator.cpp
 * @brief 游戏模型生成服务实现
 */

#include "GameModelGenerator.h"

GameModel* GameModelGenerator::generateGameModel(const LevelConfig& levelConfig)
{
    GameModel* gameModel = GameModel::create();
    if (!gameModel) {
        return nullptr;
    }
    
    int cardId = 0;
    
    // 创建主牌区卡牌
    for (const auto& cardConfig : levelConfig.playfield) {
        Card* card = _createCardFromConfig(cardConfig, cardId);
        if (card) {
            gameModel->addPlayfieldCard(card);
        }
    }
    
    // 创建堆牌区卡牌
    for (const auto& cardConfig : levelConfig.stack) {
        Card* card = _createCardFromConfig(cardConfig, cardId);
        if (card) {
            gameModel->addStackCard(card);
        }
    }
    
    // 设置游戏为就绪状态
    gameModel->setGameState(GameStateType::IDLE);
    
    return gameModel;
}

Card* GameModelGenerator::_createCardFromConfig(const CardConfig& cardConfig, int& cardId)
{
    Card* card = Card::create(cardId++, cardConfig.cardFace, cardConfig.cardSuit);
    if (card) {
        card->setPosition(cardConfig.position);
        card->setVisible(true);
    }
    return card;
}
