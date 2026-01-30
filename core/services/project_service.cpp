#include "project_service.h"

ProjectService::ProjectService(ProjectRepo& r) : repo(r) {}

bool ProjectService::isBlank(const QString& s) {
    return s.trimmed().isEmpty();
}

Result<qint64> ProjectService::add(const QString& code, const QString& name, const QString& notes) {
    if (isBlank(code)) return Result<qint64>::fail("project.code is required");
    if (isBlank(name)) return Result<qint64>::fail("project.name is required");

    Project p;
    p.code = code.trimmed();
    p.name = name.trimmed();
    p.notes = notes.trimmed();

    return Result<qint64>::success(repo.create(p));
}

ResultVoid ProjectService::update(qint64 id, const QString& code, const QString& name, const QString& notes) {
    if (!repo.get(id).has_value()) return ResultVoid::fail("project not found");
    if (isBlank(code)) return ResultVoid::fail("project.code is required");
    if (isBlank(name)) return ResultVoid::fail("project.name is required");

    Project p;
    p.id = id;
    p.code = code.trimmed();
    p.name = name.trimmed();
    p.notes = notes.trimmed();

    if (!repo.update(id, p)) return ResultVoid::fail("project update failed");
    return ResultVoid::success();
}

Result<Project> ProjectService::get(qint64 id) const {
    auto p = repo.get(id);
    if (!p.has_value()) return Result<Project>::fail("project not found");
    return Result<Project>::success(p.value());
}

Result<QList<Project>> ProjectService::list() const {
    return Result<QList<Project>>::success(repo.list());
}
