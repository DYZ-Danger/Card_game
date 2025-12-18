# 卡牌游戏项目框架文档

## 项目结构概述

该项目采用 **MVC 架构** 与分层设计模式，将系统分为以下几个主要层次：

```
Classes/
├── configs/          # 配置层 - 静态配置和枚举
├── models/           # 数据模型层 - 游戏数据和状态
├── views/            # 视图层 - UI 显示和交互
├── controllers/      # 控制器层 - 业务逻辑和流程控制
├── managers/         # 管理器层 - 特定功能管理
├── services/         # 服务层 - 无状态服务
└── utils/            # 工具层 - 通用辅助函数
```

---

## 各层详细说明

### 1. 配置层 (configs/)

**职责**：存储所有静态配置和常量定义

**主要文件**：
- `CardEnums.h` - 卡牌枚举（花色、牌面、游戏状态等）
- `GameConfig.h` - 游戏全局配置常量（分辨率、区域尺寸、动画时长等）
- `LevelConfig.h` - 关卡配置数据结构

**特点**：
- 不包含业务逻辑
- 纯数据定义和常量
- 被其他层广泛引用

---

### 2. 数据模型层 (models/)

**职责**：定义游戏的核心数据结构，存储游戏运行时状态

**主要文件**：
- `Card.h/cpp` - 单张卡牌的数据模型
  - 存储卡牌属性：花色、牌面、位置、可见性等
  - 提供卡牌匹配逻辑
  
- `GameModel.h/cpp` - 整个游戏的数据模型
  - 管理主牌区和堆牌区的卡牌集合
  - 提供卡牌查询和管理方法
  
- `UndoModel.h/cpp` - 撤销操作的数据记录
  - 存储撤销历史

**特点**：
- 只负责数据存储，不含业务逻辑
- 支持序列化/反序列化（为未来存档功能预留）
- 被 Controllers 和 Managers 使用

---

### 3. 视图层 (views/)

**职责**：负责 UI 显示、动画和用户交互

**主要文件**：
- `CardView.h/cpp` - 单张卡牌的 UI 显示
  - 处理卡牌的移动动画
  - 处理卡牌的翻转动画
  - 响应触摸事件
  
- `GameView.h/cpp` - 游戏主 UI
  - 管理主牌区和堆牌区的 UI 布局
  - 管理所有卡牌视图
  - 注册和分发事件回调

**特点**：
- 只负责显示逻辑，不包含业务规则
- 可持有 const 类型的 Model 和 Controller 指针（只读引用）
- 通过回调函数与 Controller 通信

**回调示例**：
```cpp
// View 注册回调到 Controller
gameView->setOnCardClickCallback([this](int cardId) {
    controller->handleCardClick(cardId);
});
```

---

### 4. 控制器层 (controllers/)

**职责**：协调 Model 和 View，处理业务逻辑

**主要文件**：
- `GameController.h/cpp` - 游戏主控制器
  - 初始化游戏（加载模型、创建视图）
  - 处理卡牌点击逻辑
  - 处理撤销逻辑
  - 管理游戏状态流转

**特点**：
- 包含主要的业务逻辑
- 持有 Model、View 和 Manager 的指针
- 在 Model 变化时更新 View
- 在 View 交互时修改 Model

**典型流程**：
```cpp
// 用户点击卡牌 -> View 触发回调 -> Controller 处理逻辑 -> 更新 Model -> 调用 View 动画
bool GameController::handleCardClick(int cardId) {
    Card* card = _gameModel->findCardById(cardId);
    Card* topCard = _gameModel->getTopStackCard();
    
    if (card && card->canMatch(topCard)) {
        // 记录撤销
        _undoManager->recordUndo(undoRecord);
        
        // 更新模型
        _gameModel->removePlayfieldCard(card);
        
        // 调用视图动画
        _gameView->getCardViewById(cardId)->playMoveAnimation(...);
    }
}
```

---

### 5. 管理器层 (managers/)

**职责**：提供特定功能的管理和服务

**主要文件**：
- `UndoManager.h/cpp` - 撤销功能管理
  - 持有 UndoModel
  - 执行撤销操作
  - 恢复游戏状态

**特点**：
- 作为 Controller 的成员变量
- 可持有并加工 Model 数据
- **禁止单例模式**（一个游戏实例一个 Manager）
- **禁止反向依赖 Controller**
- 通过回调接口与其他模块通信

**使用示例**：
```cpp
// Controller 中的用法
_undoManager->recordUndo(record);
_undoManager->setOnUndoCompleteCallback([this](const UndoRecord& record) {
    // 在这里处理撤销完成后的视图更新
});
```

---

### 6. 服务层 (services/)

**职责**：提供无状态的服务，处理复杂的数据转换和生成

**主要文件**：
- `GameModelGenerator.h/cpp` - 游戏模型生成服务
  - 将 LevelConfig 转换为 GameModel
  - 处理卡牌创建和初始化

**特点**：
- **不持有任何数据**（无状态）
- 通过参数操作数据或返回新数据
- 可实现为单例或静态方法
- 提供复用的公共功能

**使用示例**：
```cpp
// 服务是无状态的，直接调用静态方法
GameModel* model = GameModelGenerator::generateGameModel(levelConfig);
```

---

### 7. 工具层 (utils/)

**职责**：提供通用的辅助功能

**主要文件**：
- `LevelConfigLoader.h/cpp` - 关卡配置加载工具
  - 从 JSON 文件加载配置
  - 解析配置数据

**特点**：
- 完全独立，不涉及业务逻辑
- 可被任何层调用
- 提供通用的工具功能

---

## 核心工作流程

### 游戏初始化流程

```
用户启动游戏（GameScene）
↓
GameScene::initWithLevelId(levelId)
↓
LevelConfigLoader::loadLevelConfig(levelId)  [Utils 加载配置]
↓
GameController::create() 创建控制器
↓
GameController::initWithLevelConfig()
  ├─ GameModelGenerator::generateGameModel()  [Service 生成模型]
  │  └─ 创建 GameModel 和所有 Card
  ├─ GameView::create(gameModel)  [创建视图]
  │  └─ 为每张卡牌创建 CardView
  ├─ UndoManager::create(gameModel)  [创建管理器]
  │
  └─ 绑定所有回调函数
↓
GameController::startGame()
↓
游戏开始
```

### 用户交互流程（以点击卡牌为例）

```
用户点击卡牌
↓
CardView 检测到触摸事件
↓
CardView 触发注册的回调函数
↓
GameView 的触摸处理器调用 _onCardClickCallback(cardId)
↓
GameController::handleCardClick(cardId) [处理业务逻辑]
  ├─ 检查卡牌是否能匹配
  ├─ 记录撤销信息 _undoManager->recordUndo()  [Manager 记录]
  ├─ 更新模型 _gameModel->removePlayfieldCard()  [Model 修改]
  └─ 调用视图动画 _gameView->getCardViewById()->playMoveAnimation()  [View 显示]
↓
动画完成，游戏更新
```

### 撤销操作流程

```
用户点击撤销按钮
↓
GameView 的撤销按钮触发回调
↓
GameController::handleUndo()
↓
UndoManager::executeUndo()  [Manager 执行撤销]
  ├─ 从 UndoModel 弹出最后一条记录
  ├─ 应用撤销逻辑（恢复卡牌位置等）
  └─ 触发撤销完成回调
↓
Controller 的撤销完成回调处理视图更新
  ├─ 调用 CardView 的反向移动动画
  └─ 恢复卡牌可见性等
↓
游戏恢复到上一步的状态
```

---

## 如何扩展项目

### 1. 添加新的卡牌类型

**步骤**：
1. 在 `CardEnums.h` 中添加新的枚举值（如新的花色或特殊卡牌）
2. 在 `Card.h/cpp` 中添加相应的处理逻辑
3. 在 `CardView.h/cpp` 中添加新的视觉显示
4. 更新 `LevelConfigLoader` 来支持新的配置格式

**示例**：添加"万能牌"类型
```cpp
// CardEnums.h
enum class CardFaceType {
    // ... 现有牌面
    WILD = 20,  // 新增万能牌
};

// Card.cpp
bool Card::canMatch(Card* other) const {
    // 万能牌可以匹配任何卡牌
    if (_face == CardFaceType::WILD || other->_face == CardFaceType::WILD) {
        return true;
    }
    return calculateFaceDifference(other) == 1;
}

// CardView.cpp
Sprite* CardView::_loadCardTexture() {
    if (_cardModel->getFace() == CardFaceType::WILD) {
        // 加载万能牌纹理
        return Sprite::create("cards/wild.png");
    }
    // ... 其他卡牌
}
```

### 2. 添加新的撤销操作类型

**步骤**：
1. 在 `UndoModel.h` 的 `UndoRecord::OperationType` 中添加新的操作类型
2. 在 `GameController` 中记录新的撤销信息
3. 在 `UndoManager::_applyUndoRecord()` 中处理新的撤销逻辑

**示例**：添加"卡牌翻转"撤销
```cpp
// UndoModel.h
struct UndoRecord {
    enum class OperationType {
        CARD_MOVE = 0,
        CARD_FLIP = 1,
        CARD_DRAW = 2  // 新增：从备用牌堆抽牌
    };
};

// GameController.cpp
// 在某个操作中记录
UndoRecord record;
record.operationType = UndoRecord::OperationType::CARD_DRAW;
_undoManager->recordUndo(record);

// UndoManager.cpp
void UndoManager::_applyUndoRecord(const UndoRecord& record) {
    switch (record.operationType) {
        case UndoRecord::OperationType::CARD_DRAW:
            // 处理抽牌的撤销逻辑
            break;
        // ... 其他类型
    }
}
```

### 3. 添加新的游戏功能（如得分系统）

**步骤**：
1. 在 `GameModel` 中添加得分数据成员
2. 创建新的 Service（如 `ScoreCalculator`）来计算得分
3. 在 `GameController` 中调用 Service 更新得分
4. 在 `GameView` 中显示得分

**示例框架**：
```cpp
// models/GameModel.h 中添加
int _score;
void setScore(int score) { _score = score; }
int getScore() const { return _score; }

// services/ScoreCalculator.h（新建）
class ScoreCalculator {
public:
    static int calculateScore(Card* matchedCard, int comboCount);
};

// controllers/GameController.cpp 中使用
if (card->canMatch(topStackCard)) {
    int points = ScoreCalculator::calculateScore(card, _comboCount);
    _gameModel->setScore(_gameModel->getScore() + points);
    _gameView->updateScore(_gameModel->getScore());
}

// views/GameView.h/cpp 中显示得分
void GameView::updateScore(int score) {
    // 更新 UI 显示得分
}
```

### 4. 保存/加载游戏进度

**步骤**：
1. 在 `models` 中实现序列化方法：`Model::toJson()` 和 `Model::fromJson()`
2. 创建 `SaveSystem` 服务来管理保存和加载
3. 在 `GameController` 中调用保存功能

**示例框架**：
```cpp
// models/GameModel.h 中添加
std::string toJson() const;
static GameModel* fromJson(const std::string& jsonStr);

// services/SaveSystem.h（新建）
class SaveSystem {
public:
    static bool saveGame(GameModel* model, const std::string& filename);
    static GameModel* loadGame(const std::string& filename);
};

// 使用
SaveSystem::saveGame(_gameModel, "savegame.json");
GameModel* loadedModel = SaveSystem::loadGame("savegame.json");
```

---

## 编码规范总结

- **类名文件名**：大写字母开头（如 `GameController.h`）
- **函数和变量**：驼峰命名，小写字母开头（如 `handleCardClick`）
- **私有成员**：下划线开头（如 `_gameModel`）
- **常量**：小写 `k` 开头（如 `kCardWidth`）
- **类注释**：每个类必须在声明处添加详细注释
- **代码限制**：函数超过 50 行或类超过 500 行时需要重构

---

## 下一步开发计划

1. **实现完整的卡牌绘制系统** - 使用真实卡牌图片替代占位符
2. **实现备用牌堆和抽牌逻辑** - 添加 StackController 处理卡牌堆管理
3. **实现游戏胜利/失败检测** - 添加胜利条件判断
4. **实现得分系统** - 根据匹配数和组合数计算得分
5. **实现音效和粒子效果** - 增加游戏的视觉和音听反馈
6. **实现保存/加载功能** - 支持游戏进度保存
7. **实现多关卡系统** - 支持关卡选择和进度管理

---

## 联系和支持

如有任何问题或建议，请参考代码中的详细注释或查看对应的实现文件。
