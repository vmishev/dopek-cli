#include "process_plan_service.h"

ProcessPlanService::ProcessPlanService(ProcessPlanRepo& plans, PartRepo& parts)
    : plans_(plans), parts_(parts) {}

Result<Id> ProcessPlanService::add(Id part_id, const QString& name) {
    if (!parts_.get(part_id)) return Result<Id>::failure("part not found");
    ProcessPlan p;
    p.part_id = part_id;
    p.name = name.trimmed();
    const Id id = plans_.create(p);
    return Result<Id>::success(id);
}

ResultVoid ProcessPlanService::edit(Id id, const QString& name) {
    auto ex = plans_.get(id);
    if (!ex) return ResultVoid::failure("process plan not found");
    ProcessPlan p = *ex;
    if (!name.trimmed().isEmpty()) p.name = name.trimmed();
    if (!plans_.update(id, p)) return ResultVoid::failure("update failed");
    return ResultVoid::success();
}

std::optional<ProcessPlan> ProcessPlanService::get(Id id) const { return plans_.get(id); }
QList<ProcessPlan> ProcessPlanService::list() const { return plans_.list(); }
QList<ProcessPlan> ProcessPlanService::list_by_part(Id part_id) const { return plans_.list_by_part(part_id); }
ResultVoid ProcessPlanService::remove(Id id) {
    if (!plans_.remove(id)) return ResultVoid::failure("process plan not found");
    return ResultVoid::success();
}
