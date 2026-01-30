#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include <QtGlobal>

#include "../models/material.h"

class MaterialRepo {
public:
    qint64 create(const Material& m);
    bool update(qint64 id, const Material& m);
    std::optional<Material> get(qint64 id) const;
    bool remove(qint64 id);
    QList<Material> list() const;

private:
    QHash<qint64, Material> data;
    qint64 next_id = 1;
};
