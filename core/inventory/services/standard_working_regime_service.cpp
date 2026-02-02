#include "standard_working_regime_service.h"

StandardWorkingRegimeService::StandardWorkingRegimeService(StandardWorkingRegimeRepo& regimes) : regimes_(regimes) {}

Result<Id> StandardWorkingRegimeService::add(const StandardWorkingRegime& regime_copy) {
    StandardWorkingRegime r = regime_copy;
    r.id = 0;
    return Result<Id>::success(regimes_.create(r));
}

ResultVoid StandardWorkingRegimeService::edit(Id id, const StandardWorkingRegime& regime_copy) {
    auto ex = regimes_.get(id);
    if (!ex) return ResultVoid::failure("standard working regime not found");
    StandardWorkingRegime r = regime_copy;
    r.id = id;
    if (!regimes_.update(id, r)) return ResultVoid::failure("update failed");
    return ResultVoid::success();
}

std::optional<StandardWorkingRegime> StandardWorkingRegimeService::get(Id id) const { return regimes_.get(id); }
QList<StandardWorkingRegime> StandardWorkingRegimeService::list() const { return regimes_.list(); }
QList<StandardWorkingRegime> StandardWorkingRegimeService::list_by_operation_code(const QString& operation_code) const {
    return regimes_.list_by_operation_code(operation_code);
}
std::optional<StandardWorkingRegime> StandardWorkingRegimeService::find_best_match(const QString& operation_code,
                                                                                   const QString& material_group,
                                                                                   const QString& tool_class,
                                                                                   const QString& machine_class) const {
    return regimes_.find_best_match(operation_code, material_group, tool_class, machine_class);
}
ResultVoid StandardWorkingRegimeService::remove(Id id) {
    if (!regimes_.remove(id)) return ResultVoid::failure("standard working regime not found");
    return ResultVoid::success();
}
