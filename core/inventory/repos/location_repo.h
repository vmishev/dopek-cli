#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/location.h"

class LocationRepo {
public:
    Id create(const Location& obj);
    std::optional<Location> get(Id id) const;
    bool update(Id id, const Location& obj);
    bool remove(Id id);
    QList<Location> list() const;
    QList<Location> list_by_parent(Id parent_id) const;

private:
    QHash<Id, Location> data_;
    Id nextId_ = 1;
};
