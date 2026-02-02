#pragma once
#include <QString>
#include "../../common/util/ids.h"

struct Operation {
    Id id = 0;
    Id process_plan_id = 0;
    QString operation_code;     // use OperationCode.code for now
    QString params_json;        // minimal flexibility
    double planned_setup_min = 0.0;
    double planned_run_min = 0.0;
    double actual_setup_min = 0.0;
    double actual_run_min = 0.0;
    int sequence_no = 0;
};
