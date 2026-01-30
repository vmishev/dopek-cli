#include "material_service.h"

MaterialService::MaterialService(MaterialRepo& r) : repo(r) {}

bool MaterialService::isBlank(const QString& s) {
    return s.trimmed().isEmpty();
}

Result<qint64> MaterialService::add(const QString& name, const QString& spec, const QString& unit) {
    if (isBlank(name)) return Result<qint64>::fail("material.name is required");
    if (isBlank(unit)) return Result<qint64>::fail("material.unit is required");

    Material m;
    m.name = name.trimmed();
    m.spec = spec.trimmed();
    m.unit = unit.trimmed();

    return Result<qint64>::success(repo.create(m));
}

ResultVoid MaterialService::update(qint64 id, const QString& name, const QString& spec, const QString& unit) {
    if (!repo.get(id).has_value()) return ResultVoid::fail("material not found");
    if (isBlank(name)) return ResultVoid::fail("material.name is required");
    if (isBlank(unit)) return ResultVoid::fail("material.unit is required");

    Material m;
    m.id = id;
    m.name = name.trimmed();
    m.spec = spec.trimmed();
    m.unit = unit.trimmed();

    if (!repo.update(id, m)) return ResultVoid::fail("material update failed");
    return ResultVoid::success();
}

Result<Material> MaterialService::get(qint64 id) const {
    auto m = repo.get(id);
    if (!m.has_value()) return Result<Material>::fail("material not found");
    return Result<Material>::success(m.value());
}

Result<QList<Material>> MaterialService::list() const {
    return Result<QList<Material>>::success(repo.list());
}
