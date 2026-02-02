#pragma once
#include <QString>
#include <QDate>
#include "../../common/util/ids.h"

struct RateCard {
    Id id = 0;
    double hourly_rate = 0.0;
    QDate effective_from;
    QString notes;
};
