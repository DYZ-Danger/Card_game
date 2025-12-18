/**
 * @file UndoModel.h
 * @brief 撤销数据模型
 * @details 存储撤销操作的历史记录
 */

#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "cocos2d.h"
#include "Card.h"

USING_NS_CC;

/**
 * @brief 单个撤销操作的记录
 */
struct UndoRecord
{
    enum class OperationType
    {
        CARD_MOVE = 0,      // 卡牌移动
        CARD_FLIP = 1       // 卡牌翻转
    };
    
    OperationType operationType;    // 操作类型
    int sourceCardId;               // 源卡牌ID
    int targetCardId;               // 目标卡牌ID（如果有）
    Vec2 sourcePosition;            // 源位置
    Vec2 targetPosition;            // 目标位置
    bool sourceVisible;             // 源可见性
    bool targetVisible;             // 目标可见性
    
    UndoRecord() : operationType(OperationType::CARD_MOVE), 
                   sourceCardId(-1), targetCardId(-1),
                   sourceVisible(true), targetVisible(true) {}
};

/**
 * @brief 撤销数据模型
 * @details 管理所有撤销操作的历史记录
 */
class UndoModel
{
public:
    /**
     * @brief 创建撤销模型
     * @return UndoModel* 撤销模型指针
     */
    static UndoModel* create();
    
    /**
     * @brief 添加撤销记录
     * @param record 撤销记录
     */
    void addRecord(const UndoRecord& record);
    
    /**
     * @brief 获取最后一条记录并删除
     * @return UndoRecord 最后一条撤销记录
     */
    UndoRecord popLastRecord();
    
    /**
     * @brief 获取最后一条记录（不删除）
     * @return const UndoRecord* 最后一条撤销记录指针
     */
    const UndoRecord* getLastRecord() const;
    
    /**
     * @brief 检查是否还有可撤销的操作
     * @return bool 是否还有记录
     */
    bool hasRecord() const;
    
    /**
     * @brief 获取记录数量
     * @return int 撤销记录数
     */
    int getRecordCount() const { return (int)_records.size(); }
    
    /**
     * @brief 清空所有撤销记录
     */
    void clearAll() { _records.clear(); }
    
private:
    UndoModel();
    
    std::vector<UndoRecord> _records;   // 撤销记录栈
};

#endif // __UNDO_MODEL_H__
