#include "material_repo.h"

Id MaterialRepo::create(const Material& obj) {
    Material x = obj; x.id = nextId_++;
    data_.insert(x.id, x);
    return x.id;
}

std::optional<Material> MaterialRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}

bool MaterialRepo::update(Id id, const Material& obj) {
    if (!data_.contains(id)) return false;
    Material x = obj; x.id = id;
    data_.insert(id, x);
    return true;
}

bool MaterialRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<Material> MaterialRepo::list() const { return data_.values(); }
