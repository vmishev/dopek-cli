#include "inventory_balance_repo.h"

Id InventoryBalanceRepo::create(const InventoryBalance& obj) {
    InventoryBalance x = obj; x.id = nextId_++;
    data_.insert(x.id, x);
    return x.id;
}

std::optional<InventoryBalance> InventoryBalanceRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}

bool InventoryBalanceRepo::update(Id id, const InventoryBalance& obj) {
    if (!data_.contains(id)) return false;
    InventoryBalance x = obj; x.id = id;
    data_.insert(id, x);
    return true;
}

bool InventoryBalanceRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<InventoryBalance> InventoryBalanceRepo::list() const { return data_.values(); }

QList<InventoryBalance> InventoryBalanceRepo::list_by_material(Id material_id) const {
    QList<InventoryBalance> out;
    for (const auto& v : data_) if (v.material_id == material_id) out.append(v);
    return out;
}

QList<InventoryBalance> InventoryBalanceRepo::list_by_location(Id location_id) const {
    QList<InventoryBalance> out;
    for (const auto& v : data_) if (v.location_id == location_id) out.append(v);
    return out;
}

std::optional<InventoryBalance> InventoryBalanceRepo::get_by_material_location(Id material_id, Id location_id) const {
    for (const auto& v : data_) {
        if (v.material_id == material_id && v.location_id == location_id) return v;
    }
    return std::nullopt;
}
