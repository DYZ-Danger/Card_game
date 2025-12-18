/**
 * @file UndoManager.cpp
 * @brief 撤销管理器实现
 */

#include "UndoManager.h"

UndoManager* UndoManager::create(GameModel* gameModel)
{
    UndoManager* manager = new UndoManager(gameModel);
    return manager;
}

UndoManager::UndoManager(GameModel* gameModel)
    : _undoModel(UndoModel::create())
    , _gameModel(gameModel)
{
}

UndoManager::~UndoManager()
{
    if (_undoModel) {
        delete _undoModel;
        _undoModel = nullptr;
    }
}

void UndoManager::recordUndo(const UndoRecord& record)
{
    if (_undoModel) {
        _undoModel->addRecord(record);
    }
}

bool UndoManager::executeUndo()
{
    if (!_undoModel || !hasUndo() || !_gameModel) {
        return false;
    }
    
    UndoRecord record = _undoModel->popLastRecord();
    _applyUndoRecord(record);
    
    if (_onUndoCompleteCallback) {
        _onUndoCompleteCallback(record);
    }
    
    return true;
}

bool UndoManager::hasUndo() const
{
    return _undoModel && _undoModel->hasRecord();
}

void UndoManager::clearAll()
{
    if (_undoModel) {
        _undoModel->clearAll();
    }
}

void UndoManager::setOnUndoCompleteCallback(const std::function<void(const UndoRecord&)>& callback)
{
    _onUndoCompleteCallback = callback;
}

void UndoManager::_applyUndoRecord(const UndoRecord& record)
{
    // 这里实现撤销逻辑，根据记录类型恢复状态
    // 具体实现由controller的使用方决定，这里只提供框架
    
    // 在实际应用中，可能需要：
    // 1. 恢复卡牌位置
    // 2. 恢复卡牌可见性
    // 3. 更新游戏模型状态
}
