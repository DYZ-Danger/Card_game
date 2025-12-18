/**
 * @file UndoModel.cpp
 * @brief 撤销数据模型实现
 */

#include "UndoModel.h"

UndoModel* UndoModel::create()
{
    return new UndoModel();
}

UndoModel::UndoModel()
{
}

void UndoModel::addRecord(const UndoRecord& record)
{
    _records.push_back(record);
}

UndoRecord UndoModel::popLastRecord()
{
    if (_records.empty()) {
        return UndoRecord();
    }
    
    UndoRecord record = _records.back();
    _records.pop_back();
    return record;
}

const UndoRecord* UndoModel::getLastRecord() const
{
    if (_records.empty()) {
        return nullptr;
    }
    return &_records.back();
}

bool UndoModel::hasRecord() const
{
    return !_records.empty();
}
