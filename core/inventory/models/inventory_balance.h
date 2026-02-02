#pragma once
#include "../../common/util/ids.h"

struct InventoryBalance {
    Id id = 0;
    Id material_id = 0;
    Id location_id = 0;
    double qty = 0.0;
};
