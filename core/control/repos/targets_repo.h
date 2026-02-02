#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/targets.h"

class TargetsRepo {
public:
    Id create(const Targets& obj);
    std::optional<Targets> get(Id id) const;
    bool update(Id id, const Targets& obj);
    bool remove(Id id);
    QList<Targets> list() const;

private:
    QHash<Id, Targets> data_;
    Id nextId_ = 1;
};
