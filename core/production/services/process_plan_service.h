#pragma once
#include "../../common/services/result.h"
#include "../repos/process_plan_repo.h"
#include "../repos/part_repo.h"

class ProcessPlanService {
public:
    ProcessPlanService(ProcessPlanRepo& plans, PartRepo& parts);

    Result<Id> add(Id part_id, const QString& name);
    ResultVoid edit(Id id, const QString& name);
    std::optional<ProcessPlan> get(Id id) const;
    QList<ProcessPlan> list() const;
    QList<ProcessPlan> list_by_part(Id part_id) const;
    ResultVoid remove(Id id);

private:
    ProcessPlanRepo& plans_;
    PartRepo& parts_;
};
