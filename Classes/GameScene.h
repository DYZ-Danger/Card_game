/**
 * @file GameScene.h
 * @brief 游戏场景
 * @details 演示如何使用GameController
 */

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "controllers/GameController.h"

USING_NS_CC;

/**
 * @brief 游戏场景
 */
class GameScene : public Scene
{
public:
    /**
     * @brief 创建游戏场景
     * @param levelId 关卡ID
     * @return GameScene* 游戏场景指针
     */
    static GameScene* create(int levelId);
    
    /**
     * @brief 初始化场景
     * @param levelId 关卡ID
     * @return bool 是否初始化成功
     */
    bool initWithLevelId(int levelId);
    
protected:
    GameScene();
    virtual ~GameScene();
    
private:
    GameController* _gameController;    // 游戏控制器
    
    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__
