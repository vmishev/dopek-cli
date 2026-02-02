#include "overhead_repo.h"

Id OverheadRepo::create(const Overhead& obj) {
    Overhead x = obj; x.id = nextId_++;
    data_.insert(x.id, x);
    return x.id;
}
std::optional<Overhead> OverheadRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}
bool OverheadRepo::update(Id id, const Overhead& obj) {
    if (!data_.contains(id)) return false;
    Overhead x = obj; x.id = id;
    data_.insert(id, x);
    return true;
}
bool OverheadRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<Overhead> OverheadRepo::list() const { return data_.values(); }
