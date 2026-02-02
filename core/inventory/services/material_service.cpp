#include "material_service.h"

MaterialService::MaterialService(MaterialRepo& materials) : materials_(materials) {}

Result<Id> MaterialService::add(const QString& code, const QString& group, const QString& name, const QString& notes) {
    Material m;
    m.code = code.trimmed();
    m.group = group.trimmed();
    m.name = name.trimmed();
    m.notes = notes.trimmed();
    return Result<Id>::success(materials_.create(m));
}

ResultVoid MaterialService::edit(Id id, const QString& code, const QString& group, const QString& name, const QString& notes) {
    auto ex = materials_.get(id);
    if (!ex) return ResultVoid::failure("material not found");
    Material m = *ex;
    if (!code.trimmed().isEmpty()) m.code = code.trimmed();
    if (!group.trimmed().isEmpty()) m.group = group.trimmed();
    if (!name.trimmed().isEmpty()) m.name = name.trimmed();
    if (!notes.isNull()) m.notes = notes.trimmed();
    if (!materials_.update(id, m)) return ResultVoid::failure("update failed");
    return ResultVoid::success();
}

std::optional<Material> MaterialService::get(Id id) const { return materials_.get(id); }
QList<Material> MaterialService::list() const { return materials_.list(); }
ResultVoid MaterialService::remove(Id id) {
    if (!materials_.remove(id)) return ResultVoid::failure("material not found");
    return ResultVoid::success();
}
