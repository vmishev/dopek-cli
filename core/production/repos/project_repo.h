#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/project.h"

class ProjectRepo {
public:
    Id create(const Project& obj);
    std::optional<Project> get(Id id) const;
    bool update(Id id, const Project& obj);
    bool remove(Id id);
    QList<Project> list() const;

private:
    QHash<Id, Project> data_;
    Id nextId_ = 1;
};
