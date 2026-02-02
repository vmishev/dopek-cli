#pragma once
#include <QString>
#include "../../common/util/ids.h"

struct Part {
    Id id = 0;
    QString code;
    QString name;
    QString drawing_ref;
};
