#include "project_service.h"

ProjectService::ProjectService(ProjectRepo& projects) : projects_(projects) {}

Result<Id> ProjectService::add(const QString& name, const QString& description) {
    Project p;
    p.name = name.trimmed();
    p.description = description.trimmed();
    const Id id = projects_.create(p);
    return Result<Id>::success(id);
}

ResultVoid ProjectService::edit(Id id, const QString& name, const QString& description) {
    auto ex = projects_.get(id);
    if (!ex) return ResultVoid::failure("project not found");
    Project p = *ex;
    if (!name.trimmed().isEmpty()) p.name = name.trimmed();
    p.description = description.trimmed();
    if (!projects_.update(id, p)) return ResultVoid::failure("update failed");
    return ResultVoid::success();
}

std::optional<Project> ProjectService::get(Id id) const { return projects_.get(id); }
QList<Project> ProjectService::list() const { return projects_.list(); }
ResultVoid ProjectService::remove(Id id) {
    if (!projects_.remove(id)) return ResultVoid::failure("project not found");
    return ResultVoid::success();
}
