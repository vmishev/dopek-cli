#pragma once
#include "../../common/services/result.h"
#include "../repos/rate_card_repo.h"

class RateCardService {
public:
    explicit RateCardService(RateCardRepo& rates);

    Result<Id> add(const RateCard& copy);
    ResultVoid edit(Id id, const RateCard& copy);
    std::optional<RateCard> get(Id id) const;
    QList<RateCard> list() const;
    ResultVoid remove(Id id);

private:
    RateCardRepo& rates_;
};
