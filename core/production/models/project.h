#pragma once
#include <QString>
#include <QDate>
#include "../../common/util/ids.h"

struct Project {
    Id id = 0;
    QString name;
    QString description;
    QDate created_at;
};
