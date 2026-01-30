#pragma once
#include <QString>
#include <QList>
#include <QtGlobal>

#include "../services/result.h"
#include "../models/material.h"
#include "../repos/material_repo.h"

class MaterialService {
public:
    explicit MaterialService(MaterialRepo& repo);

    Result<qint64> add(const QString& name, const QString& spec, const QString& unit);
    ResultVoid update(qint64 id, const QString& name, const QString& spec, const QString& unit);

    Result<Material> get(qint64 id) const;
    Result<QList<Material>> list() const;

private:
    MaterialRepo& repo;
    static bool isBlank(const QString& s);
};
