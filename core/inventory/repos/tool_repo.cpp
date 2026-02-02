#include "tool_repo.h"

Id ToolRepo::create(const Tool& obj) {
    Tool x = obj; x.id = nextId_++;
    data_.insert(x.id, x);
    return x.id;
}

std::optional<Tool> ToolRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}

bool ToolRepo::update(Id id, const Tool& obj) {
    if (!data_.contains(id)) return false;
    Tool x = obj; x.id = id;
    data_.insert(id, x);
    return true;
}

bool ToolRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<Tool> ToolRepo::list() const { return data_.values(); }
