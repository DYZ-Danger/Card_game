/**
 * @file GameView.h
 * @brief 游戏主视图
 * @details 管理整个游戏UI的显示，包括主牌区、堆牌区等
 */

#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "CardView.h"
#include "../models/GameModel.h"

USING_NS_CC;

/**
 * @brief 游戏主视图类
 * @details 负责整个游戏界面的UI布局和显示
 */
class GameView : public Node
{
public:
    /**
     * @brief 创建游戏视图
     * @param gameModel 游戏数据模型
     * @return GameView* 游戏视图指针
     */
    static GameView* create(GameModel* gameModel);
    
    /**
     * @brief 初始化视图布局
     */
    virtual bool init(GameModel* gameModel);
    
    /**
     * @brief 添加卡牌视图
     * @param cardView 卡牌视图
     * @param isPlayfield 是否添加到主牌区
     */
    void addCardView(CardView* cardView, bool isPlayfield = true);
    
    /**
     * @brief 移除卡牌视图
     * @param cardView 卡牌视图
     */
    void removeCardView(CardView* cardView);
    
    /**
     * @brief 获取卡牌视图（通过ID）
     * @param cardId 卡牌ID
     * @return CardView* 卡牌视图指针
     */
    CardView* getCardViewById(int cardId) const;
    
    /**
     * @brief 获取主牌区节点
     * @return Node* 主牌区节点
     */
    Node* getPlayfieldNode() const { return _playfieldNode; }
    
    /**
     * @brief 获取堆牌区节点
     * @return Node* 堆牌区节点
     */
    Node* getStackNode() const { return _stackNode; }
    
    /**
     * @brief 注册回调函数（卡牌被点击）
     * @param callback 回调函数
     */
    void setOnCardClickCallback(const std::function<void(int)>& callback);
    
    /**
     * @brief 注册回调函数（撤销按钮被点击）
     * @param callback 回调函数
     */
    void setOnUndoClickCallback(const std::function<void()>& callback);
    
protected:
    GameView();
    virtual ~GameView();
    
    /**
     * @brief 初始化UI布局
     */
    void _initializeLayout();
    
    /**
     * @brief 初始化按钮
     */
    void _initializeButtons();
    
    /**
     * @brief 创建主牌区
     */
    void _createPlayfieldArea();
    
    /**
     * @brief 创建堆牌区
     */
    void _createStackArea();
    
    /**
     * @brief 处理触摸事件
     */
    bool _onTouchBegan(Touch* touch, Event* event);
    
private:
    GameModel* _gameModel;              // 关联的游戏模型
    Node* _playfieldNode;               // 主牌区节点
    Node* _stackNode;                   // 堆牌区节点
    
    std::map<int, CardView*> _cardViews;    // 所有卡牌视图的映射
    
    std::function<void(int)> _onCardClickCallback;      // 卡牌点击回调
    std::function<void()> _onUndoClickCallback;         // 撤销点击回调
};

#endif // __GAME_VIEW_H__
