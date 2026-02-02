#pragma once
#include "../../common/services/result.h"
#include "../repos/machine_repo.h"

class MachineService {
public:
    explicit MachineService(MachineRepo& machines);

    Result<Id> add(const QString& code, const QString& name, const QString& machine_class, const QString& notes);
    ResultVoid edit(Id id, const QString& code, const QString& name, const QString& machine_class, const QString& notes);
    std::optional<Machine> get(Id id) const;
    QList<Machine> list() const;
    ResultVoid remove(Id id);

private:
    MachineRepo& machines_;
};
