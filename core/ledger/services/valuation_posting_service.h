#pragma once
#include "../../common/services/result.h"
#include "../repos/valuation_posting_repo.h"

class ValuationPostingService {
public:
    explicit ValuationPostingService(ValuationPostingRepo& postings);

    Result<Id> add(const ValuationPosting& copy);
    ResultVoid edit(Id id, const ValuationPosting& copy);
    std::optional<ValuationPosting> get(Id id) const;
    QList<ValuationPosting> list() const;
    ResultVoid remove(Id id);

private:
    ValuationPostingRepo& postings_;
};
