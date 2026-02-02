#include "overhead_service.h"

OverheadService::OverheadService(OverheadRepo& overheads) : overheads_(overheads) {}

Result<Id> OverheadService::add(const Overhead& copy) {
    Overhead o = copy; o.id = 0;
    return Result<Id>::success(overheads_.create(o));
}

ResultVoid OverheadService::edit(Id id, const Overhead& copy) {
    auto ex = overheads_.get(id);
    if (!ex) return ResultVoid::failure("overhead not found");
    Overhead o = copy; o.id = id;
    if (!overheads_.update(id, o)) return ResultVoid::failure("update failed");
    return ResultVoid::success();
}

std::optional<Overhead> OverheadService::get(Id id) const { return overheads_.get(id); }
QList<Overhead> OverheadService::list() const { return overheads_.list(); }
ResultVoid OverheadService::remove(Id id) {
    if (!overheads_.remove(id)) return ResultVoid::failure("overhead not found");
    return ResultVoid::success();
}
