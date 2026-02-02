#pragma once
#include "../../common/services/result.h"
#include "../repos/overhead_repo.h"

class OverheadService {
public:
    explicit OverheadService(OverheadRepo& overheads);

    Result<Id> add(const Overhead& copy);
    ResultVoid edit(Id id, const Overhead& copy);
    std::optional<Overhead> get(Id id) const;
    QList<Overhead> list() const;
    ResultVoid remove(Id id);

private:
    OverheadRepo& overheads_;
};
