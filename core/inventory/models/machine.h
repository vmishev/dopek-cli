#pragma once
#include <QString>
#include "../../common/util/ids.h"

struct Machine {
    Id id = 0;
    QString code;
    QString name;
    QString machine_class;
    QString notes;
};
