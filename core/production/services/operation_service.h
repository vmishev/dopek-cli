#pragma once
#include "../../common/services/result.h"
#include "../repos/operation_repo.h"
#include "../repos/process_plan_repo.h"
#include "../repos/operation_code_repo.h"

class OperationService {
public:
    OperationService(OperationRepo& ops, ProcessPlanRepo& plans, OperationCodeRepo& codes);

    Result<Id> add(Id process_plan_id,
                   const QString& operation_code,
                   int sequence_no,
                   const QString& params_json);

    ResultVoid edit(Id id,
                    const QString& operation_code,
                    std::optional<int> sequence_no,
                    const QString& params_json);

    std::optional<Operation> get(Id id) const;
    QList<Operation> list() const;
    QList<Operation> list_by_process_plan(Id process_plan_id) const;
    ResultVoid remove(Id id);

private:
    OperationRepo& ops_;
    ProcessPlanRepo& plans_;
    OperationCodeRepo& codes_;
};
