#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/inventory_balance.h"

class InventoryBalanceRepo {
public:
    Id create(const InventoryBalance& obj);
    std::optional<InventoryBalance> get(Id id) const;
    bool update(Id id, const InventoryBalance& obj);
    bool remove(Id id);
    QList<InventoryBalance> list() const;

    QList<InventoryBalance> list_by_material(Id material_id) const;
    QList<InventoryBalance> list_by_location(Id location_id) const;
    std::optional<InventoryBalance> get_by_material_location(Id material_id, Id location_id) const;

private:
    QHash<Id, InventoryBalance> data_;
    Id nextId_ = 1;
};
