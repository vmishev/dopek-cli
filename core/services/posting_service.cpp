#include "posting_service.h"

PostingService::PostingService(PostingRepo& p, InvoiceLineRepo& l)
    : postings(p), lines(l) {}

bool PostingService::isPostingTypeValid(const QString& s) {
    const QString v = s.trimmed();
    return (v == "project" || v == "inventory");
}

Result<qint64> PostingService::add(qint64 invoice_line_id,
                                   const QString& posting_type,
                                   qint64 project_id,
                                   qint64 material_id) {
    if (!lines.get(invoice_line_id).has_value())
        return Result<qint64>::fail("posting.invoice_line_id not found");

    if (!isPostingTypeValid(posting_type))
        return Result<qint64>::fail("posting.posting_type invalid");

    if (posting_type == "project" && project_id == 0)
        return Result<qint64>::fail("posting.project_id required");

    if (posting_type == "inventory" && material_id == 0)
        return Result<qint64>::fail("posting.material_id required");

    Posting p;
    p.invoice_line_id = invoice_line_id;
    p.posting_type = posting_type.trimmed();
    p.project_id = project_id;
    p.material_id = material_id;

    return Result<qint64>::success(postings.create(p));
}

ResultVoid PostingService::remove(qint64 id) {
    if (!postings.remove(id))
        return ResultVoid::fail("posting not found");
    return ResultVoid::success();
}

Result<QList<Posting>> PostingService::list_by_invoice_line(qint64 invoice_line_id) const {
    return Result<QList<Posting>>::success(postings.list_by_invoice_line(invoice_line_id));
}
