/**
 * @file GameModel.cpp
 * @brief 游戏全局数据模型实现
 */

#include "GameModel.h"

GameModel* GameModel::create()
{
    return new GameModel();
}

GameModel::GameModel()
    : _gameState(GameStateType::IDLE)
{
}

GameModel::~GameModel()
{
    clearAll();
}

void GameModel::addPlayfieldCard(Card* card)
{
    if (!card) return;
    _playfieldCards.push_back(card);
    card->setArea(CardAreaType::PLAYFIELD);
}

void GameModel::addStackCard(Card* card)
{
    if (!card) return;
    card->setStackIndex(_stackCards.size());
    _stackCards.push_back(card);
    card->setArea(CardAreaType::STACK);
}

Card* GameModel::getTopStackCard() const
{
    if (_stackCards.empty()) return nullptr;
    return _stackCards.back();
}

void GameModel::removePlayfieldCard(Card* card)
{
    if (!card) return;
    auto it = std::find(_playfieldCards.begin(), _playfieldCards.end(), card);
    if (it != _playfieldCards.end()) {
        _playfieldCards.erase(it);
    }
}

void GameModel::removeStackCard(Card* card)
{
    if (!card) return;
    auto it = std::find(_stackCards.begin(), _stackCards.end(), card);
    if (it != _stackCards.end()) {
        _stackCards.erase(it);
    }
}

Card* GameModel::findCardById(int cardId) const
{
    for (auto card : _playfieldCards) {
        if (card->getCardId() == cardId) return card;
    }
    for (auto card : _stackCards) {
        if (card->getCardId() == cardId) return card;
    }
    return nullptr;
}

void GameModel::clearAll()
{
    for (auto card : _playfieldCards) {
        delete card;
    }
    for (auto card : _stackCards) {
        delete card;
    }
    _playfieldCards.clear();
    _stackCards.clear();
}

int GameModel::getTotalCardCount() const
{
    return (int)(_playfieldCards.size() + _stackCards.size());
}
