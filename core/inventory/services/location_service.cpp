#include "location_service.h"

LocationService::LocationService(LocationRepo& locations) : locations_(locations) {}

Result<Id> LocationService::add(const QString& name, Id parent_id) {
    Location l;
    l.name = name.trimmed();
    l.parent_id = parent_id;
    return Result<Id>::success(locations_.create(l));
}

ResultVoid LocationService::edit(Id id, const QString& name, std::optional<Id> parent_id) {
    auto ex = locations_.get(id);
    if (!ex) return ResultVoid::failure("location not found");
    Location l = *ex;
    if (!name.trimmed().isEmpty()) l.name = name.trimmed();
    if (parent_id.has_value()) l.parent_id = *parent_id;
    if (!locations_.update(id, l)) return ResultVoid::failure("update failed");
    return ResultVoid::success();
}

std::optional<Location> LocationService::get(Id id) const { return locations_.get(id); }
QList<Location> LocationService::list() const { return locations_.list(); }
QList<Location> LocationService::list_by_parent(Id parent_id) const { return locations_.list_by_parent(parent_id); }
ResultVoid LocationService::remove(Id id) {
    if (!locations_.remove(id)) return ResultVoid::failure("location not found");
    return ResultVoid::success();
}
