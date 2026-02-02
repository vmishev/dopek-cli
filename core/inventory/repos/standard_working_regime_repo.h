#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/standard_working_regime.h"

class StandardWorkingRegimeRepo {
public:
    Id create(const StandardWorkingRegime& obj);
    std::optional<StandardWorkingRegime> get(Id id) const;
    bool update(Id id, const StandardWorkingRegime& obj);
    bool remove(Id id);
    QList<StandardWorkingRegime> list() const;

    QList<StandardWorkingRegime> list_by_operation_code(const QString& operation_code) const;
    std::optional<StandardWorkingRegime> find_best_match(const QString& operation_code,
                                                         const QString& material_group,
                                                         const QString& tool_class,
                                                         const QString& machine_class) const;

private:
    QHash<Id, StandardWorkingRegime> data_;
    Id nextId_ = 1;
};
