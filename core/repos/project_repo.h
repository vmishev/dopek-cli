#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include <QtGlobal>

#include "../models/project.h"

class ProjectRepo {
public:
    qint64 create(const Project& p);
    bool update(qint64 id, const Project& p);
    std::optional<Project> get(qint64 id) const;
    bool remove(qint64 id);
    QList<Project> list() const;

private:
    QHash<qint64, Project> data;
    qint64 next_id = 1;
};
