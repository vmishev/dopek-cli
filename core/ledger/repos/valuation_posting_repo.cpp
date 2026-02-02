#include "valuation_posting_repo.h"

Id ValuationPostingRepo::create(const ValuationPosting& obj) {
    ValuationPosting p = obj; p.id = nextId_++;
    data_.insert(p.id, p);
    return p.id;
}

std::optional<ValuationPosting> ValuationPostingRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}

bool ValuationPostingRepo::update(Id id, const ValuationPosting& obj) {
    if (!data_.contains(id)) return false;
    ValuationPosting p = obj; p.id = id;
    data_.insert(id, p);
    return true;
}

bool ValuationPostingRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<ValuationPosting> ValuationPostingRepo::list() const { return data_.values(); }
