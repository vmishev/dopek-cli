#pragma once
#include <QString>
#include "../../common/util/ids.h"

struct Overhead {
    Id id = 0;
    double monthly_total = 0.0;
    QString notes;
};
