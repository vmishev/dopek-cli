#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/rate_card.h"

class RateCardRepo {
public:
    Id create(const RateCard& obj);
    std::optional<RateCard> get(Id id) const;
    bool update(Id id, const RateCard& obj);
    bool remove(Id id);
    QList<RateCard> list() const;

private:
    QHash<Id, RateCard> data_;
    Id nextId_ = 1;
};
