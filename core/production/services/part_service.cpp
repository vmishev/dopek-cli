#include "part_service.h"

PartService::PartService(PartRepo& parts) : parts_(parts) {}

Result<Id> PartService::add(const QString& code, const QString& name, const QString& drawing_ref) {
    Part p;
    p.code = code.trimmed();
    p.name = name.trimmed();
    p.drawing_ref = drawing_ref.trimmed();
    const Id id = parts_.create(p);
    return Result<Id>::success(id);
}

ResultVoid PartService::edit(Id id, const QString& code, const QString& name, const QString& drawing_ref) {
    auto ex = parts_.get(id);
    if (!ex) return ResultVoid::failure("part not found");
    Part p = *ex;
    if (!code.trimmed().isEmpty()) p.code = code.trimmed();
    if (!name.trimmed().isEmpty()) p.name = name.trimmed();
    p.drawing_ref = drawing_ref.trimmed();
    if (!parts_.update(id, p)) return ResultVoid::failure("update failed");
    return ResultVoid::success();
}

std::optional<Part> PartService::get(Id id) const { return parts_.get(id); }
QList<Part> PartService::list() const { return parts_.list(); }
ResultVoid PartService::remove(Id id) {
    if (!parts_.remove(id)) return ResultVoid::failure("part not found");
    return ResultVoid::success();
}
