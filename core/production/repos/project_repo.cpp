#include "project_repo.h"

Id ProjectRepo::create(const Project& obj) {
    Project x = obj; x.id = nextId_++;
    data_.insert(x.id, x);
    return x.id;
}

std::optional<Project> ProjectRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}

bool ProjectRepo::update(Id id, const Project& obj) {
    if (!data_.contains(id)) return false;
    Project x = obj; x.id = id;
    data_.insert(id, x);
    return true;
}

bool ProjectRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<Project> ProjectRepo::list() const { return data_.values(); }
