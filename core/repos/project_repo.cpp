#include "project_repo.h"

qint64 ProjectRepo::create(const Project& p) {
    Project copy = p;
    copy.id = next_id;
    data.insert(next_id, copy);
    return next_id++;
}

bool ProjectRepo::update(qint64 id, const Project& p) {
    if (!data.contains(id)) return false;
    Project copy = p;
    copy.id = id;
    data[id] = copy;
    return true;
}

std::optional<Project> ProjectRepo::get(qint64 id) const {
    if (!data.contains(id)) return std::nullopt;
    return data.value(id);
}

bool ProjectRepo::remove(qint64 id) {
    return data.remove(id) > 0;
}

QList<Project> ProjectRepo::list() const {
    return data.values();
}
