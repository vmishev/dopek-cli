#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/operation_code.h"

class OperationCodeRepo {
public:
    Id create(const OperationCode& obj);
    std::optional<OperationCode> get(Id id) const;
    bool update(Id id, const OperationCode& obj);
    bool remove(Id id);
    QList<OperationCode> list() const;
    std::optional<OperationCode> get_by_code(const QString& code) const;

private:
    QHash<Id, OperationCode> data_;
    Id nextId_ = 1;
};
