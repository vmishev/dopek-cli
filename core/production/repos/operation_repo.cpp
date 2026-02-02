#include "operation_repo.h"

Id OperationRepo::create(const Operation& obj) {
    Operation x = obj; x.id = nextId_++;
    data_.insert(x.id, x);
    return x.id;
}
std::optional<Operation> OperationRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}
bool OperationRepo::update(Id id, const Operation& obj) {
    if (!data_.contains(id)) return false;
    Operation x = obj; x.id = id;
    data_.insert(id, x);
    return true;
}
bool OperationRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<Operation> OperationRepo::list() const { return data_.values(); }

QList<Operation> OperationRepo::list_by_process_plan(Id process_plan_id) const {
    QList<Operation> out;
    for (const auto& v : data_) if (v.process_plan_id == process_plan_id) out.append(v);
    return out;
}
