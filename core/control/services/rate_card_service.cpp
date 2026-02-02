#include "rate_card_service.h"

RateCardService::RateCardService(RateCardRepo& rates) : rates_(rates) {}

Result<Id> RateCardService::add(const RateCard& copy) {
    RateCard r = copy; r.id = 0;
    return Result<Id>::success(rates_.create(r));
}

ResultVoid RateCardService::edit(Id id, const RateCard& copy) {
    auto ex = rates_.get(id);
    if (!ex) return ResultVoid::failure("rate card not found");
    RateCard r = copy; r.id = id;
    if (!rates_.update(id, r)) return ResultVoid::failure("update failed");
    return ResultVoid::success();
}

std::optional<RateCard> RateCardService::get(Id id) const { return rates_.get(id); }
QList<RateCard> RateCardService::list() const { return rates_.list(); }
ResultVoid RateCardService::remove(Id id) {
    if (!rates_.remove(id)) return ResultVoid::failure("rate card not found");
    return ResultVoid::success();
}
