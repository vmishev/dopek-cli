#pragma once
#include "../../common/services/result.h"
#include "../repos/targets_repo.h"

class TargetsService {
public:
    explicit TargetsService(TargetsRepo& targets);

    Result<Id> add(const Targets& copy);
    ResultVoid edit(Id id, const Targets& copy);
    std::optional<Targets> get(Id id) const;
    QList<Targets> list() const;
    ResultVoid remove(Id id);

private:
    TargetsRepo& targets_;
};
