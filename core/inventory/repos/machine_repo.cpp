#include "machine_repo.h"

Id MachineRepo::create(const Machine& obj) {
    Machine x = obj; x.id = nextId_++;
    data_.insert(x.id, x);
    return x.id;
}

std::optional<Machine> MachineRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}

bool MachineRepo::update(Id id, const Machine& obj) {
    if (!data_.contains(id)) return false;
    Machine x = obj; x.id = id;
    data_.insert(id, x);
    return true;
}

bool MachineRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<Machine> MachineRepo::list() const { return data_.values(); }
