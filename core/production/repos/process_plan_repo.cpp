#include "process_plan_repo.h"

Id ProcessPlanRepo::create(const ProcessPlan& obj) {
    ProcessPlan x = obj; x.id = nextId_++;
    data_.insert(x.id, x);
    return x.id;
}
std::optional<ProcessPlan> ProcessPlanRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}
bool ProcessPlanRepo::update(Id id, const ProcessPlan& obj) {
    if (!data_.contains(id)) return false;
    ProcessPlan x = obj; x.id = id;
    data_.insert(id, x);
    return true;
}
bool ProcessPlanRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<ProcessPlan> ProcessPlanRepo::list() const { return data_.values(); }

QList<ProcessPlan> ProcessPlanRepo::list_by_part(Id part_id) const {
    QList<ProcessPlan> out;
    for (const auto& v : data_) if (v.part_id == part_id) out.append(v);
    return out;
}
