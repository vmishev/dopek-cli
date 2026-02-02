#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/valuation_posting.h"

class ValuationPostingRepo {
public:
    Id create(const ValuationPosting& obj);
    std::optional<ValuationPosting> get(Id id) const;
    bool update(Id id, const ValuationPosting& obj);
    bool remove(Id id);
    QList<ValuationPosting> list() const;

private:
    QHash<Id, ValuationPosting> data_;
    Id nextId_ = 1;
};
