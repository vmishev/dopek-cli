#pragma once
#include <QString>
#include "../../common/util/ids.h"

struct Location {
    Id id = 0;
    QString name;          // e.g. "Cabinet A / Shelf 2"
    Id parent_id = 0;      // 0 = root
};
