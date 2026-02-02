#pragma once
#include "../../common/util/ids.h"

struct Targets {
    Id id = 0;
    int buffer_target_months = 0;   // e.g. 24
    double margin_target = 0.0;     // e.g. 0.35
    double utilization_target = 0.0; // e.g. 0.70
};
