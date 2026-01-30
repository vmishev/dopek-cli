#include "inventory_service.h"

InventoryService::InventoryService(InventoryRepo& i, MaterialRepo& m)
    : inventory(i), materials(m) {}

Result<qint64> InventoryService::set(qint64 material_id, double qty) {
    if (!materials.get(material_id).has_value())
        return Result<qint64>::fail("inventory.material_id not found");
    if (qty < 0.0)
        return Result<qint64>::fail("inventory.qty must be >= 0");

    return Result<qint64>::success(inventory.upsert_by_material(material_id, qty));
}

Result<Inventory> InventoryService::get(qint64 material_id) const {
    auto inv = inventory.get_by_material(material_id);
    if (!inv.has_value()) return Result<Inventory>::fail("inventory not found");
    return Result<Inventory>::success(inv.value());
}

Result<QList<Inventory>> InventoryService::list() const {
    return Result<QList<Inventory>>::success(inventory.list());
}
