#include "part_repo.h"

Id PartRepo::create(const Part& obj) {
    Part x = obj; x.id = nextId_++;
    data_.insert(x.id, x);
    return x.id;
}
std::optional<Part> PartRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}
bool PartRepo::update(Id id, const Part& obj) {
    if (!data_.contains(id)) return false;
    Part x = obj; x.id = id;
    data_.insert(id, x);
    return true;
}
bool PartRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<Part> PartRepo::list() const { return data_.values(); }
