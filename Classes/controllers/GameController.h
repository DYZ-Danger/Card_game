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

/**
 * @brief 游戏主控制器
 * @details 控制整个游戏的逻辑流程，协调模型和视图
 */
class GameController : public Ref
{
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
    
private:
    GameModel* _gameModel;              // 游戏数据模型
    GameView* _gameView;                // 游戏视图
    UndoManager* _undoManager;          // 撤销管理器
    
    int _selectedCardId;                // 当前选中的卡牌ID
    GameStateType _currentGameState;    // 当前游戏状态
};

#endif // __GAME_CONTROLLER_H__
