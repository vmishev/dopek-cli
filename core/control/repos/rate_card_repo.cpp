#include "rate_card_repo.h"

Id RateCardRepo::create(const RateCard& obj) {
    RateCard x = obj; x.id = nextId_++;
    data_.insert(x.id, x);
    return x.id;
}
std::optional<RateCard> RateCardRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}
bool RateCardRepo::update(Id id, const RateCard& obj) {
    if (!data_.contains(id)) return false;
    RateCard x = obj; x.id = id;
    data_.insert(id, x);
    return true;
}
bool RateCardRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<RateCard> RateCardRepo::list() const { return data_.values(); }
