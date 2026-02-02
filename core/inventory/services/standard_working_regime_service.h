#pragma once
#include "../../common/services/result.h"
#include "../repos/standard_working_regime_repo.h"

class StandardWorkingRegimeService {
public:
    explicit StandardWorkingRegimeService(StandardWorkingRegimeRepo& regimes);

    Result<Id> add(const StandardWorkingRegime& regime_copy);
    ResultVoid edit(Id id, const StandardWorkingRegime& regime_copy);
    std::optional<StandardWorkingRegime> get(Id id) const;
    QList<StandardWorkingRegime> list() const;
    QList<StandardWorkingRegime> list_by_operation_code(const QString& operation_code) const;
    std::optional<StandardWorkingRegime> find_best_match(const QString& operation_code,
                                                         const QString& material_group,
                                                         const QString& tool_class,
                                                         const QString& machine_class) const;
    ResultVoid remove(Id id);

private:
    StandardWorkingRegimeRepo& regimes_;
};
