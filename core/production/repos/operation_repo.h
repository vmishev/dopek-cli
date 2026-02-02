#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/operation.h"

class OperationRepo {
public:
    Id create(const Operation& obj);
    std::optional<Operation> get(Id id) const;
    bool update(Id id, const Operation& obj);
    bool remove(Id id);
    QList<Operation> list() const;
    QList<Operation> list_by_process_plan(Id process_plan_id) const;

private:
    QHash<Id, Operation> data_;
    Id nextId_ = 1;
};
