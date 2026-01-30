#include "material_repo.h"

qint64 MaterialRepo::create(const Material& m) {
    Material copy = m;
    copy.id = next_id;
    data.insert(next_id, copy);
    return next_id++;
}

bool MaterialRepo::update(qint64 id, const Material& m) {
    if (!data.contains(id)) return false;
    Material copy = m;
    copy.id = id;
    data[id] = copy;
    return true;
}

std::optional<Material> MaterialRepo::get(qint64 id) const {
    if (!data.contains(id)) return std::nullopt;
    return data.value(id);
}

bool MaterialRepo::remove(qint64 id) {
    return data.remove(id) > 0;
}

QList<Material> MaterialRepo::list() const {
    return data.values();
}
