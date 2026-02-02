#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/material.h"

class MaterialRepo {
public:
    Id create(const Material& obj);
    std::optional<Material> get(Id id) const;
    bool update(Id id, const Material& obj);
    bool remove(Id id);
    QList<Material> list() const;

private:
    QHash<Id, Material> data_;
    Id nextId_ = 1;
};
