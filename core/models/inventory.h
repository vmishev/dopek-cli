// core/models/inventory.h
#pragma once
#include <QtGlobal>

struct Inventory {
    qint64 id = 0;
    qint64 material_id = 0;
    double qty = 0.0;
};
