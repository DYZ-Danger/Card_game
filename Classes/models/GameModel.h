/**
 * @file GameModel.h
 * @brief 游戏全局数据模型
 * @details 存储游戏运行时的整体状态和数据
 */

#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "cocos2d.h"
#include "Card.h"
#include "../configs/CardEnums.h"

USING_NS_CC;

/**
 * @brief 游戏数据模型
 * @details 管理游戏的所有卡牌数据、游戏状态等
 */
class GameModel
{
public:
    /**
     * @brief 创建游戏模型
     * @return GameModel* 游戏模型指针
     */
    static GameModel* create();
    
    // 游戏状态管理
    void setGameState(GameStateType state) { _gameState = state; }
    GameStateType getGameState() const { return _gameState; }
    
    // 卡牌管理
    /**
     * @brief 添加卡牌到主牌区
     * @param card 卡牌对象
     */
    void addPlayfieldCard(Card* card);
    
    /**
     * @brief 添加卡牌到堆牌区
     * @param card 卡牌对象
     */
    void addStackCard(Card* card);
    
    /**
     * @brief 获取所有主牌区卡牌
     * @return const std::vector<Card*>& 主牌区卡牌列表
     */
    const std::vector<Card*>& getPlayfieldCards() const { return _playfieldCards; }
    
    /**
     * @brief 获取所有堆牌区卡牌
     * @return const std::vector<Card*>& 堆牌区卡牌列表
     */
    const std::vector<Card*>& getStackCards() const { return _stackCards; }
    
    /**
     * @brief 获取当前顶部卡牌（堆牌区最上面的牌）
     * @return Card* 顶部卡牌，如果没有返回nullptr
     */
    Card* getTopStackCard() const;
    
    /**
     * @brief 移除主牌区卡牌
     * @param card 卡牌对象
     */
    void removePlayfieldCard(Card* card);
    
    /**
     * @brief 移除堆牌区卡牌
     * @param card 卡牌对象
     */
    void removeStackCard(Card* card);
    
    /**
     * @brief 通过ID查找卡牌
     * @param cardId 卡牌ID
     * @return Card* 找到的卡牌，未找到返回nullptr
     */
    Card* findCardById(int cardId) const;
    
    /**
     * @brief 清空所有卡牌数据
     */
    void clearAll();
    
    /**
     * @brief 获取卡牌总数
     * @return int 卡牌总数
     */
    int getTotalCardCount() const;
    
private:
    GameModel();
    virtual ~GameModel();
    
    GameStateType _gameState;               // 当前游戏状态
    std::vector<Card*> _playfieldCards;     // 主牌区卡牌
    std::vector<Card*> _stackCards;         // 堆牌区卡牌（栈结构，最后一个是顶部）
};

#endif // __GAME_MODEL_H__
