/**
 * @file GameController.h
 * @brief 游戏主控制器
 * @details 协调整个游戏流程，管理模型和视图的交互
 */

#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "../views/GameView.h"
#include "../managers/UndoManager.h"
#include "../configs/LevelConfig.h"

USING_NS_CC;

// 前向声明
class UndoManager;

/**
 * @brief 游戏主控制器
 * @details 控制整个游戏的逻辑流程，协调模型和视图
 */
class GameController : public Ref
{
    // 友元类声明
    friend class UndoManager;
    
public:
    /**
     * @brief 创建游戏控制器
     * @return GameController* 控制器指针
     */
    static GameController* create();
    
    /**
     * @brief 初始化游戏（根据关卡配置）
     * @param levelConfig 关卡配置
     * @return bool 是否初始化成功
     */
    bool initWithLevelConfig(const LevelConfig& levelConfig);
    
    /**
     * @brief 获取游戏模型
     * @return GameModel* 游戏模型
     */
    GameModel* getGameModel() const { return _gameModel; }
    
    /**
     * @brief 获取游戏视图
     * @return GameView* 游戏视图
     */
    GameView* getGameView() const { return _gameView; }
    
    /**
     * @brief 处理卡牌点击事件
     * @param cardId 被点击的卡牌ID
     * @return bool 是否成功处理
     */
    bool handleCardClick(int cardId);
    
    /**
     * @brief 处理撤销请求
     * @return bool 是否成功撤销
     */
    bool handleUndo();
    
    /**
     * @brief 启动游戏
     */
    void startGame();
    
    /**
     * @brief 停止游戏
     */
    void stopGame();
    
    /**
     * @brief 重启游戏
     */
    void restartGame();
    
protected:
    GameController();
    virtual ~GameController();
    
    /**
     * @brief 初始化控制器内部结构
     */
    bool _initialize();
    
    /**
     * @brief 初始化模型
     */
    bool _initializeModel(const LevelConfig& levelConfig);
    
    /**
     * @brief 初始化视图
     */
    bool _initializeView();
    
    /**
     * @brief 绑定事件回调
     */
    void _bindCallbacks();
    
    /**
     * @brief 处理Playfield卡牌点击（匹配逻辑）
     * @param cardId 被点击的卡牌ID
     * @param clickedCard 被点击的卡牌指针
     * @return bool 是否成功处理
     */
    bool _handlePlayfieldCardClick(int cardId, Card* clickedCard);
    
    /**
     * @brief 处理Stack卡牌点击（补牌逻辑）
     * @param cardId 被点击的卡牌ID
     * @param clickedCard 被点击的卡牌指针
     * @return bool 是否成功处理
     */
    bool _handleStackCardClick(int cardId, Card* clickedCard);
    
    /**
     * @brief 刷新Stack显示（更新位置和视图）
     */
    void _refreshStackDisplay();
    
    /**
     * @brief 执行撤销操作（由UndoManager回调）
     * @param record 撤销记录
     */
    void _applyUndo(const UndoRecord& record);
    
    /**
     * @brief 撤销Playfield→Stack的匹配操作
     * @param record 撤销记录
     */
    void _undoPlayfieldToStack(const UndoRecord& record);
    
    /**
     * @brief 撤销Stack补牌操作
     * @param record 撤销记录
     */
    void _undoStackSupplement(const UndoRecord& record);
    
    /**
     * @brief 创建Playfield匹配的撤销记录
     * @param cardId 点击的卡牌ID
     * @param clickedCard 点击的卡牌
     * @param rightStackCard Stack右边的质底牌
     * @return UndoRecord 撤销记录
     */
    UndoRecord _createMatchUndoRecord(int cardId, Card* clickedCard, Card* rightStackCard);
    
    /**
     * @brief 执行卡牌匹配动画和后续处理
     * @param cardId 点击的卡牌ID
     * @param clickedCard 点击的卡牌
     * @param rightStackCard Stack右边的质底牌
     */
    void _executeMatchAnimation(int cardId, Card* clickedCard, Card* rightStackCard);
    
    /**
     * @brief 执行匹配后的模型更新
     * @param clickedCard 点击的卡牌
     * @param rightStackCard Stack右边的质底牌
     */
    void _updateModelAfterMatch(Card* clickedCard, Card* rightStackCard);
    
    /**
     * @brief 移除当前Stack中所有卡牌的视图
     */
    void _removeAllStackViews();
    
    /**
     * @brief 根据撤销记录恢复Stack卡牌
     * @param record 撤销记录
     */
    void _restoreStackFromRecord(const UndoRecord& record);
    
    /**
     * @brief 恢复Playfield卡牌
     * @param record 撤销记录
     */
    void _restorePlayfieldCard(const UndoRecord& record);
    
    /**
     * @brief 重建Stack的所有视图
     */
    void _rebuildStackViews();
    
    /**
     * @brief 为Stack卡牌创建并添加视图（左边堆+右边单独）
     * @param stackCards Stack中的所有卡牌
     */
    void _createStackCardViews(const std::vector<Card*>& stackCards);

private:
    GameModel* _gameModel;              // 游戏数据模型
    GameView* _gameView;                // 游戏视图
    UndoManager* _undoManager;          // 撤销管理器
    
    int _selectedCardId;                // 当前选中的卡牌ID
    GameStateType _currentGameState;    // 当前游戏状态
};

#endif // __GAME_CONTROLLER_H__
