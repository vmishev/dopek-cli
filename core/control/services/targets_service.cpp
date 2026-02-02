#include "targets_service.h"

TargetsService::TargetsService(TargetsRepo& targets) : targets_(targets) {}

Result<Id> TargetsService::add(const Targets& copy) {
    Targets t = copy; t.id = 0;
    return Result<Id>::success(targets_.create(t));
}

ResultVoid TargetsService::edit(Id id, const Targets& copy) {
    auto ex = targets_.get(id);
    if (!ex) return ResultVoid::failure("targets not found");
    Targets t = copy; t.id = id;
    if (!targets_.update(id, t)) return ResultVoid::failure("update failed");
    return ResultVoid::success();
}

std::optional<Targets> TargetsService::get(Id id) const { return targets_.get(id); }
QList<Targets> TargetsService::list() const { return targets_.list(); }
ResultVoid TargetsService::remove(Id id) {
    if (!targets_.remove(id)) return ResultVoid::failure("targets not found");
    return ResultVoid::success();
}
