#include "inventory_repo.h"

qint64 InventoryRepo::upsert_by_material(qint64 material_id, double qty) {
    if (by_material.contains(material_id)) {
        Inventory inv = by_material.value(material_id);
        inv.material_id = material_id;
        inv.qty = qty;
        by_material[material_id] = inv;
        return inv.id;
    }

    Inventory inv;
    inv.id = next_id++;
    inv.material_id = material_id;
    inv.qty = qty;
    by_material.insert(material_id, inv);
    return inv.id;
}

std::optional<Inventory> InventoryRepo::get_by_material(qint64 material_id) const {
    if (!by_material.contains(material_id)) return std::nullopt;
    return by_material.value(material_id);
}

QList<Inventory> InventoryRepo::list() const {
    return by_material.values();
}
