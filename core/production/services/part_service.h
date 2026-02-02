#pragma once
#include "../../common/services/result.h"
#include "../repos/part_repo.h"

class PartService {
public:
    explicit PartService(PartRepo& parts);

    Result<Id> add(const QString& code, const QString& name, const QString& drawing_ref);
    ResultVoid edit(Id id, const QString& code, const QString& name, const QString& drawing_ref);
    std::optional<Part> get(Id id) const;
    QList<Part> list() const;
    ResultVoid remove(Id id);

private:
    PartRepo& parts_;
};
