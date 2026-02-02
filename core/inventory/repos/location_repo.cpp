#include "location_repo.h"

Id LocationRepo::create(const Location& obj) {
    Location x = obj; x.id = nextId_++;
    data_.insert(x.id, x);
    return x.id;
}

std::optional<Location> LocationRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}

bool LocationRepo::update(Id id, const Location& obj) {
    if (!data_.contains(id)) return false;
    Location x = obj; x.id = id;
    data_.insert(id, x);
    return true;
}

bool LocationRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<Location> LocationRepo::list() const { return data_.values(); }

QList<Location> LocationRepo::list_by_parent(Id parent_id) const {
    QList<Location> out;
    for (const auto& v : data_) if (v.parent_id == parent_id) out.append(v);
    return out;
}
