#pragma once
#include <QString>
#include "../../common/util/ids.h"

struct Tool {
    Id id = 0;
    QString code;
    QString name;
    QString tool_class;
    QString notes;
};
