#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/overhead.h"

class OverheadRepo {
public:
    Id create(const Overhead& obj);
    std::optional<Overhead> get(Id id) const;
    bool update(Id id, const Overhead& obj);
    bool remove(Id id);
    QList<Overhead> list() const;

private:
    QHash<Id, Overhead> data_;
    Id nextId_ = 1;
};
