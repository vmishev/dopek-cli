#pragma once
#include <QString>
#include "../../common/util/ids.h"

struct StandardWorkingRegime {
    Id id = 0;
    QString operation_code;     // e.g. TURN_OD
    QString material_group;     // e.g. alloy_steel
    QString tool_class;         // e.g. carbide_turning
    QString machine_class;      // e.g. lathe_manual
    double vc_m_per_min = 0.0;
    double feed = 0.0;          // interpret per op later
    double doc_mm = 0.0;
    QString notes;
    QString risk_level;         // low/medium/high (string for MVP)
};
