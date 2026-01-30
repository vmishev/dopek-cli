#pragma once
#include <QString>
#include <QList>
#include <QtGlobal>

#include "../services/result.h"
#include "../models/project.h"
#include "../repos/project_repo.h"

class ProjectService {
public:
    explicit ProjectService(ProjectRepo& repo);

    Result<qint64> add(const QString& code, const QString& name, const QString& notes);
    ResultVoid update(qint64 id, const QString& code, const QString& name, const QString& notes);

    Result<Project> get(qint64 id) const;
    Result<QList<Project>> list() const;

private:
    ProjectRepo& repo;
    static bool isBlank(const QString& s);
};
