/**
 * @file UndoManager.h
 * @brief 撤销管理器
 * @details 管理游戏撤销功能，处理撤销操作的记录和执行
 */

#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "cocos2d.h"
#include "../models/UndoModel.h"
#include "../models/GameModel.h"

USING_NS_CC;

/**
 * @brief 撤销管理器类
 * @details 主要作为controller的成员，管理撤销功能
 *          持有UndoModel数据并处理撤销逻辑
 */
class UndoManager
{
public:
    /**
     * @brief 创建撤销管理器
     * @param gameModel 游戏模型（用于恢复状态）
     * @return UndoManager* 撤销管理器指针
     */
    static UndoManager* create(GameModel* gameModel);
    
    /**
     * @brief 记录一个撤销操作
     * @param record 撤销记录
     */
    void recordUndo(const UndoRecord& record);
    
    /**
     * @brief 执行撤销（回退一步）
     * @return bool 是否成功撤销
     */
    bool executeUndo();
    
    /**
     * @brief 检查是否还有可撤销的操作
     * @return bool 是否还有记录
     */
    bool hasUndo() const;
    
    /**
     * @brief 清空所有撤销记录
     */
    void clearAll();
    
    /**
     * @brief 注册撤销完成回调
     * @param callback 回调函数
     */
    void setOnUndoCompleteCallback(const std::function<void(const UndoRecord&)>& callback);
    
protected:
    UndoManager(GameModel* gameModel);
    virtual ~UndoManager();
    
private:
    UndoModel* _undoModel;              // 撤销数据模型
    GameModel* _gameModel;              // 游戏模型（用于状态恢复）
    
    std::function<void(const UndoRecord&)> _onUndoCompleteCallback;   // 撤销完成回调
    
    /**
     * @brief 应用撤销记录，恢复游戏状态
     * @param record 撤销记录
     */
    void _applyUndoRecord(const UndoRecord& record);
};

#endif // __UNDO_MANAGER_H__
