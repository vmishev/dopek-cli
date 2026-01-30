#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include <QtGlobal>

#include "../models/inventory.h"

class InventoryRepo {
public:
    // snapshot per material_id
    qint64 upsert_by_material(qint64 material_id, double qty);

    std::optional<Inventory> get_by_material(qint64 material_id) const;
    QList<Inventory> list() const;

private:
    QHash<qint64, Inventory> by_material; // material_id -> Inventory
    qint64 next_id = 1;
};
