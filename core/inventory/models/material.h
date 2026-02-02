#pragma once
#include <QString>
#include "../../common/util/ids.h"

struct Material {
    Id id = 0;
    QString code;          // e.g. C45, 42CrMo4
    QString group;         // e.g. alloy_steel
    QString name;
    QString notes;
};
