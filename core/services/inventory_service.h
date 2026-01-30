#pragma once
#include <QList>
#include <QtGlobal>

#include "../services/result.h"
#include "../models/inventory.h"
#include "../repos/inventory_repo.h"
#include "../repos/material_repo.h"

class InventoryService {
public:
    InventoryService(InventoryRepo& inventory, MaterialRepo& materials);

    Result<qint64> set(qint64 material_id, double qty);
    Result<Inventory> get(qint64 material_id) const;
    Result<QList<Inventory>> list() const;

private:
    InventoryRepo& inventory;
    MaterialRepo& materials;
};
