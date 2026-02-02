#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/machine.h"

class MachineRepo {
public:
    Id create(const Machine& obj);
    std::optional<Machine> get(Id id) const;
    bool update(Id id, const Machine& obj);
    bool remove(Id id);
    QList<Machine> list() const;

private:
    QHash<Id, Machine> data_;
    Id nextId_ = 1;
};
