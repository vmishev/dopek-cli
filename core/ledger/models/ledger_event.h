#pragma once
#include <QString>
#include <QDateTime>
#include "../../common/util/ids.h"

struct LedgerEvent {
    Id id = 0;
    QDateTime ts;
    QString type;          // e.g. MaterialReceived, MaterialConsumed
    QString payload_json;  // flexible MVP
    QString refs_json;     // ids: project/material/invoice...
};
