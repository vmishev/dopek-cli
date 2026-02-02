#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/part.h"

class PartRepo {
public:
    Id create(const Part& obj);
    std::optional<Part> get(Id id) const;
    bool update(Id id, const Part& obj);
    bool remove(Id id);
    QList<Part> list() const;

private:
    QHash<Id, Part> data_;
    Id nextId_ = 1;
};
