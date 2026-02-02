#include "standard_working_regime_repo.h"

Id StandardWorkingRegimeRepo::create(const StandardWorkingRegime& obj) {
    StandardWorkingRegime x = obj; x.id = nextId_++;
    data_.insert(x.id, x);
    return x.id;
}

std::optional<StandardWorkingRegime> StandardWorkingRegimeRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}

bool StandardWorkingRegimeRepo::update(Id id, const StandardWorkingRegime& obj) {
    if (!data_.contains(id)) return false;
    StandardWorkingRegime x = obj; x.id = id;
    data_.insert(id, x);
    return true;
}

bool StandardWorkingRegimeRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<StandardWorkingRegime> StandardWorkingRegimeRepo::list() const { return data_.values(); }

QList<StandardWorkingRegime> StandardWorkingRegimeRepo::list_by_operation_code(const QString& operation_code) const {
    QList<StandardWorkingRegime> out;
    for (const auto& v : data_) if (v.operation_code == operation_code) out.append(v);
    return out;
}

std::optional<StandardWorkingRegime> StandardWorkingRegimeRepo::find_best_match(const QString& operation_code,
                                                                                const QString& material_group,
                                                                                const QString& tool_class,
                                                                                const QString& machine_class) const {
    // MVP: exact match only (разширение по-късно).
    for (const auto& v : data_) {
        if (v.operation_code == operation_code &&
            v.material_group == material_group &&
            v.tool_class == tool_class &&
            v.machine_class == machine_class) {
            return v;
        }
    }
    return std::nullopt;
}
