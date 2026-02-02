#pragma once
#include "../../common/services/result.h"
#include "../repos/location_repo.h"

class LocationService {
public:
    explicit LocationService(LocationRepo& locations);

    Result<Id> add(const QString& name, Id parent_id);
    ResultVoid edit(Id id, const QString& name, std::optional<Id> parent_id);
    std::optional<Location> get(Id id) const;
    QList<Location> list() const;
    QList<Location> list_by_parent(Id parent_id) const;
    ResultVoid remove(Id id);

private:
    LocationRepo& locations_;
};
