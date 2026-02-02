#pragma once
#include <QString>
#include "../../common/util/ids.h"

struct OperationCode {
    Id id = 0;
    QString code;        // e.g. TURN_OD, MILL_SLOT
    QString name;
    QString description;
};
