#include "inventory_balance_service.h"

InventoryBalanceService::InventoryBalanceService(InventoryBalanceRepo& balances, MaterialRepo& materials, LocationRepo& locations)
    : balances_(balances), materials_(materials), locations_(locations) {}

Result<Id> InventoryBalanceService::add(Id material_id, Id location_id, double qty) {
    if (!materials_.get(material_id)) return Result<Id>::failure("material not found");
    if (!locations_.get(location_id)) return Result<Id>::failure("location not found");

    InventoryBalance b;
    b.material_id = material_id;
    b.location_id = location_id;
    b.qty = qty;

    return Result<Id>::success(balances_.create(b));
}

ResultVoid InventoryBalanceService::edit(Id id, std::optional<double> qty) {
    auto ex = balances_.get(id);
    if (!ex) return ResultVoid::failure("inventory balance not found");
    InventoryBalance b = *ex;
    if (qty.has_value()) b.qty = *qty;
    if (!balances_.update(id, b)) return ResultVoid::failure("update failed");
    return ResultVoid::success();
}

std::optional<InventoryBalance> InventoryBalanceService::get(Id id) const { return balances_.get(id); }
QList<InventoryBalance> InventoryBalanceService::list() const { return balances_.list(); }
QList<InventoryBalance> InventoryBalanceService::list_by_material(Id material_id) const { return balances_.list_by_material(material_id); }
QList<InventoryBalance> InventoryBalanceService::list_by_location(Id location_id) const { return balances_.list_by_location(location_id); }
std::optional<InventoryBalance> InventoryBalanceService::get_by_material_location(Id material_id, Id location_id) const {
    return balances_.get_by_material_location(material_id, location_id);
}
ResultVoid InventoryBalanceService::remove(Id id) {
    if (!balances_.remove(id)) return ResultVoid::failure("inventory balance not found");
    return ResultVoid::success();
}
