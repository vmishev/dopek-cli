#pragma once
#include "../../common/services/result.h"
#include "../repos/inventory_balance_repo.h"
#include "../repos/material_repo.h"
#include "../repos/location_repo.h"

class InventoryBalanceService {
public:
    InventoryBalanceService(InventoryBalanceRepo& balances, MaterialRepo& materials, LocationRepo& locations);

    Result<Id> add(Id material_id, Id location_id, double qty);
    ResultVoid edit(Id id, std::optional<double> qty);
    std::optional<InventoryBalance> get(Id id) const;
    QList<InventoryBalance> list() const;
    QList<InventoryBalance> list_by_material(Id material_id) const;
    QList<InventoryBalance> list_by_location(Id location_id) const;
    std::optional<InventoryBalance> get_by_material_location(Id material_id, Id location_id) const;
    ResultVoid remove(Id id);

private:
    InventoryBalanceRepo& balances_;
    MaterialRepo& materials_;
    LocationRepo& locations_;
};
