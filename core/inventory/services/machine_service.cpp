#include "machine_service.h"

MachineService::MachineService(MachineRepo& machines) : machines_(machines) {}

Result<Id> MachineService::add(const QString& code, const QString& name, const QString& machine_class, const QString& notes) {
    Machine m;
    m.code = code.trimmed();
    m.name = name.trimmed();
    m.machine_class = machine_class.trimmed();
    m.notes = notes.trimmed();
    return Result<Id>::success(machines_.create(m));
}

ResultVoid MachineService::edit(Id id, const QString& code, const QString& name, const QString& machine_class, const QString& notes) {
    auto ex = machines_.get(id);
    if (!ex) return ResultVoid::failure("machine not found");
    Machine m = *ex;
    if (!code.trimmed().isEmpty()) m.code = code.trimmed();
    if (!name.trimmed().isEmpty()) m.name = name.trimmed();
    if (!machine_class.trimmed().isEmpty()) m.machine_class = machine_class.trimmed();
    if (!notes.isNull()) m.notes = notes.trimmed();
    if (!machines_.update(id, m)) return ResultVoid::failure("update failed");
    return ResultVoid::success();
}

std::optional<Machine> MachineService::get(Id id) const { return machines_.get(id); }
QList<Machine> MachineService::list() const { return machines_.list(); }
ResultVoid MachineService::remove(Id id) {
    if (!machines_.remove(id)) return ResultVoid::failure("machine not found");
    return ResultVoid::success();
}
