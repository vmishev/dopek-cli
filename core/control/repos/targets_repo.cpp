#include "targets_repo.h"

Id TargetsRepo::create(const Targets& obj) {
    Targets x = obj; x.id = nextId_++;
    data_.insert(x.id, x);
    return x.id;
}
std::optional<Targets> TargetsRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}
bool TargetsRepo::update(Id id, const Targets& obj) {
    if (!data_.contains(id)) return false;
    Targets x = obj; x.id = id;
    data_.insert(id, x);
    return true;
}
bool TargetsRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<Targets> TargetsRepo::list() const { return data_.values(); }
