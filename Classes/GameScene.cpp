/**
 * @file GameScene.cpp
 * @brief 游戏场景实现
 */

#include "GameScene.h"
#include "utils/LevelConfigLoader.h"

GameScene* GameScene::create(int levelId)
{
    GameScene* scene = new GameScene();
    if (scene && scene->initWithLevelId(levelId)) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

GameScene::GameScene()
    : _gameController(nullptr)
{
}

GameScene::~GameScene()
{
    if (_gameController) {
        _gameController->release();
        _gameController = nullptr;
    }
}

bool GameScene::initWithLevelId(int levelId)
{
    if (!Scene::init()) {
        return false;
    }
    
    // 加载关卡配置
    LevelConfig levelConfig = LevelConfigLoader::loadLevelConfig(levelId);
    
    // 创建游戏控制器
    _gameController = GameController::create();
    if (!_gameController) {
        return false;
    }
    
    _gameController->retain();
    
    // 初始化控制器
    if (!_gameController->initWithLevelConfig(levelConfig)) {
        return false;
    }
    
    // 获取游戏视图并添加到场景
    GameView* gameView = _gameController->getGameView();
    if (gameView) {
        addChild(gameView);
    }
    
    // 启动游戏
    _gameController->startGame();
    
    return true;
}
