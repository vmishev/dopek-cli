#pragma once
#include "../../common/services/result.h"
#include "../repos/material_repo.h"

class MaterialService {
public:
    explicit MaterialService(MaterialRepo& materials);

    Result<Id> add(const QString& code, const QString& group, const QString& name, const QString& notes);
    ResultVoid edit(Id id, const QString& code, const QString& group, const QString& name, const QString& notes);
    std::optional<Material> get(Id id) const;
    QList<Material> list() const;
    ResultVoid remove(Id id);

private:
    MaterialRepo& materials_;
};
