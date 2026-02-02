#pragma once
#include "../../common/services/result.h"
#include "../repos/project_repo.h"

class ProjectService {
public:
    explicit ProjectService(ProjectRepo& projects);

    Result<Id> add(const QString& name, const QString& description);
    ResultVoid edit(Id id, const QString& name, const QString& description);
    std::optional<Project> get(Id id) const;
    QList<Project> list() const;
    ResultVoid remove(Id id);

private:
    ProjectRepo& projects_;
};
