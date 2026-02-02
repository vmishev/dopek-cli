#pragma once
#include <QString>
#include "../../common/util/ids.h"

struct ProcessPlan {
    Id id = 0;
    Id part_id = 0;
    QString name;
};
