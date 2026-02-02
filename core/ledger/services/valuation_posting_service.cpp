#include "valuation_posting_service.h"

ValuationPostingService::ValuationPostingService(ValuationPostingRepo& postings) : postings_(postings) {}

Result<Id> ValuationPostingService::add(const ValuationPosting& copy) {
    ValuationPosting p = copy; p.id = 0;
    return Result<Id>::success(postings_.create(p));
}

ResultVoid ValuationPostingService::edit(Id id, const ValuationPosting& copy) {
    auto ex = postings_.get(id);
    if (!ex) return ResultVoid::failure("valuation posting not found");
    ValuationPosting p = copy; p.id = id;
    if (!postings_.update(id, p)) return ResultVoid::failure("update failed");
    return ResultVoid::success();
}

std::optional<ValuationPosting> ValuationPostingService::get(Id id) const { return postings_.get(id); }
QList<ValuationPosting> ValuationPostingService::list() const { return postings_.list(); }
ResultVoid ValuationPostingService::remove(Id id) {
    if (!postings_.remove(id)) return ResultVoid::failure("valuation posting not found");
    return ResultVoid::success();
}
