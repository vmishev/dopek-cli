#pragma once
#include <QtGlobal>

#include "../services/result.h"
#include "../models/posting.h"
#include "../repos/posting_repo.h"
#include "../repos/invoice_line_repo.h"

class PostingService {
public:
    PostingService(PostingRepo& postings, InvoiceLineRepo& lines);

    Result<qint64> add(qint64 invoice_line_id,
                       const QString& posting_type,
                       qint64 project_id,
                       qint64 material_id);

    ResultVoid remove(qint64 id);
    Result<QList<Posting>> list_by_invoice_line(qint64 invoice_line_id) const;

private:
    PostingRepo& postings;
    InvoiceLineRepo& lines;

    static bool isPostingTypeValid(const QString& s);
};
