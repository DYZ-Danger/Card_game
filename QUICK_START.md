# 快速开始指南

## 项目框架概览

本项目是基于 cocos2d-x 3.17 的卡牌游戏框架实现，采用 **MVC 架构** 设计。

## 项目已实现的核心组件

### ✅ 已完成

```
Classes/
├── configs/
│   ├── CardEnums.h              # 卡牌枚举定义
│   ├── GameConfig.h             # 游戏配置常量
│   └── LevelConfig.h            # 关卡配置结构
│
├── models/
│   ├── Card.h/cpp               # 卡牌数据模型
│   ├── GameModel.h/cpp          # 游戏全局数据模型
│   └── UndoModel.h/cpp          # 撤销操作记录模型
│
├── views/
│   ├── CardView.h/cpp           # 单张卡牌视图
│   └── GameView.h/cpp           # 游戏主视图
│
├── controllers/
│   └── GameController.h/cpp     # 游戏主控制器
│
├── managers/
│   └── UndoManager.h/cpp        # 撤销功能管理器
│
├── services/
│   └── GameModelGenerator.h/cpp # 游戏模型生成服务
│
└── utils/
    └── LevelConfigLoader.h/cpp  # 关卡配置加载工具
```

## 核心类及其职责

| 类名 | 所在层 | 职责 | 关键方法 |
|------|--------|------|---------|
| **Card** | models | 单张卡牌数据 | `getFace()`, `getSuit()`, `canMatch()` |
| **GameModel** | models | 游戏全局数据 | `addPlayfieldCard()`, `getTopStackCard()` |
| **UndoModel** | models | 撤销记录 | `addRecord()`, `popLastRecord()` |
| **CardView** | views | 卡牌 UI 显示 | `playMoveAnimation()`, `playFlipAnimation()` |
| **GameView** | views | 游戏 UI 布局 | `addCardView()`, `setOnCardClickCallback()` |
| **GameController** | controllers | 游戏逻辑控制 | `handleCardClick()`, `handleUndo()`, `initWithLevelConfig()` |
| **UndoManager** | managers | 撤销管理 | `recordUndo()`, `executeUndo()` |
| **GameModelGenerator** | services | 模型生成 | `generateGameModel()` |
| **LevelConfigLoader** | utils | 配置加载 | `loadLevelConfig()` |

## 使用示例

### 1. 创建并初始化游戏

```cpp
// 在 AppDelegate.cpp 中
#include "GameScene.h"
#include "utils/LevelConfigLoader.h"

bool AppDelegate::applicationDidFinishLaunching() {
    // ... cocos2d 初始化代码 ...
    
    // 创建游戏场景（第 1 关）
    Scene* scene = GameScene::create(1);
    director->runWithScene(scene);
    
    return true;
}
```

### 2. 加载关卡配置

创建 `Resources/levels/level_1.json`：

```json
{
    "levelId": 1,
    "Playfield": [
        {
            "CardFace": 12,
            "CardSuit": 0,
            "Position": {"x": 250, "y": 1000}
        },
        {
            "CardFace": 2,
            "CardSuit": 1,
            "Position": {"x": 350, "y": 600}
        }
    ],
    "Stack": [
        {
            "CardFace": 3,
            "CardSuit": 0,
            "Position": {"x": 540, "y": 750}
        }
    ]
}
```

### 3. 游戏流程

```cpp
// 游戏自动处理的流程：

// 1. 用户点击主牌区的卡牌
//    ↓ GameView 检测触摸
//    ↓ 触发 _onCardClickCallback(cardId)
//    ↓ GameController::handleCardClick(cardId) 处理

// 2. 控制器检查卡牌是否能匹配
//    - 获取点击的卡牌
//    - 获取堆顶卡牌
//    - 调用 card->canMatch(topCard) 检查

// 3. 如果能匹配，执行以下操作：
//    - 记录撤销信息到 UndoManager
//    - 更新 GameModel 中的卡牌状态
//    - 调用 CardView 执行移动动画

// 4. 用户点击撤销按钮
//    ↓ GameController::handleUndo()
//    ↓ UndoManager::executeUndo() 弹出上一步记录
//    ↓ 调用 CardView 执行反向动画
```

## 关键配置

### 设计分辨率

在 `AppDelegate::applicationDidFinishLaunching()` 中设置：

```cpp
glview = GLViewImpl::createWithRect("CardGame", 
                                   Rect(0, 0, 1080, 2080), 
                                   0.5);
glview->setDesignResolutionSize(1080, 2080, 
                                ResolutionPolicy::FIXED_WIDTH);
```

### 游戏配置常量

在 `GameConfig.h` 中修改：

```cpp
class GameConfig {
public:
    static constexpr float kDesignWidth = 1080.0f;
    static constexpr float kDesignHeight = 2080.0f;
    static constexpr float kCardWidth = 120.0f;
    static constexpr float kCardHeight = 160.0f;
    // ... 其他常量
};
```

## 花色和牌面编号对照

### 花色编号（CardSuitType）
- 0 = CLUBS（梅花）
- 1 = DIAMONDS（方块）
- 2 = HEARTS（红桃）
- 3 = SPADES（黑桃）

### 牌面编号（CardFaceType）
- 1 = ACE（A）
- 2-10 = 数字牌
- 11 = JACK（J）
- 12 = QUEEN（Q）
- 13 = KING（K）

## 主要功能说明

### ✅ 已实现的功能

1. **卡牌模型和数据管理**
   - 完整的 Card 和 GameModel 数据结构
   - 卡牌匹配逻辑（点数差 1）

2. **游戏视图和动画**
   - CardView 和 GameView 提供 UI 显示
   - 支持卡牌移动动画和翻转动画

3. **撤销功能框架**
   - UndoManager 记录操作历史
   - 支持回退到任意前一步

4. **模型加载和初始化**
   - LevelConfigLoader 从 JSON 加载配置
   - GameModelGenerator 生成游戏模型

5. **MVC 架构**
   - 完整的 Model、View、Controller 分离
   - Manager 和 Service 提供专业的数据加工和服务

### 📋 待实现的功能

1. **卡牌绘制**
   - 需要加载真实的卡牌纹理图片
   - 需要实现花色和牌面的显示

2. **堆牌区逻辑**
   - 从备用牌堆抽取新牌
   - 堆牌区的完整管理

3. **游戏胜负判定**
   - 胜利条件（清空所有卡牌）
   - 失败条件（无法继续操作）

4. **得分系统**
   - 根据操作计算得分
   - 显示得分 UI

5. **音效和特效**
   - 卡牌匹配音效
   - 胜利音效和粒子效果

## 集成到现有项目

### 步骤 1：复制文件

将 `Classes/` 下的所有目录复制到你的项目中

### 步骤 2：更新 CMakeLists.txt

在 CMakeLists.txt 中添加新的源文件：

```cmake
list(APPEND GAME_SOURCE
    # 配置层
    Classes/configs/CardEnums.h
    Classes/configs/GameConfig.h
    Classes/configs/LevelConfig.h
    
    # 模型层
    Classes/models/Card.h
    Classes/models/Card.cpp
    Classes/models/GameModel.h
    Classes/models/GameModel.cpp
    Classes/models/UndoModel.h
    Classes/models/UndoModel.cpp
    
    # 视图层
    Classes/views/CardView.h
    Classes/views/CardView.cpp
    Classes/views/GameView.h
    Classes/views/GameView.cpp
    
    # 控制器层
    Classes/controllers/GameController.h
    Classes/controllers/GameController.cpp
    
    # 管理器层
    Classes/managers/UndoManager.h
    Classes/managers/UndoManager.cpp
    
    # 服务层
    Classes/services/GameModelGenerator.h
    Classes/services/GameModelGenerator.cpp
    
    # 工具层
    Classes/utils/LevelConfigLoader.h
    Classes/utils/LevelConfigLoader.cpp
    
    # 场景
    Classes/GameScene.h
    Classes/GameScene.cpp
)
```

### 步骤 3：修改 AppDelegate

```cpp
#include "GameScene.h"

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
    if (!glview) {
        glview = GLViewImpl::createWithRect("CardGame", 
                                          Rect(0, 0, 1080, 2080), 
                                          0.5);
        glview->setDesignResolutionSize(1080, 2080, 
                                        ResolutionPolicy::FIXED_WIDTH);
        director->setOpenGLView(glview);
    }
    
    // 创建游戏场景
    Scene* scene = GameScene::create(1);
    director->runWithScene(scene);
    
    return true;
}
```

## 调试技巧

### 打印日志

```cpp
// 使用 cocos2d 的 CCLOG 打印调试信息
CCLOG("Card ID: %d, Face: %d", card->getCardId(), (int)card->getFace());

// 检查卡牌匹配
Card* card1 = ...;
Card* card2 = ...;
CCLOG("Can match: %s", card1->canMatch(card2) ? "YES" : "NO");
```

### 检查模型数据

```cpp
// 打印所有卡牌
auto cards = _gameModel->getPlayfieldCards();
CCLOG("Total cards: %d", (int)cards.size());
for (auto card : cards) {
    CCLOG("Card %d: Face %d, Suit %d", 
          card->getCardId(), 
          (int)card->getFace(), 
          (int)card->getSuit());
}
```

## 常见问题

**Q: 如何添加新的卡牌类型？**
A: 详见 `PROJECT_FRAMEWORK.md` 中的"如何扩展项目"部分

**Q: 如何修改卡牌的外观？**
A: 在 `CardView::_loadCardTexture()` 中加载不同的纹理图片

**Q: 如何支持多关卡？**
A: 为每个关卡创建对应的 JSON 配置文件，然后通过 `GameScene::create(levelId)` 加载

**Q: 如何实现保存/加载？**
A: 详见 `PROJECT_FRAMEWORK.md` 中的"添加保存/加载功能"示例

## 相关文档

- 详细架构说明：[PROJECT_FRAMEWORK.md](./PROJECT_FRAMEWORK.md)
- 需求文档：[description.md](./description.md)

---

**祝你开发顺利！** 🎮
