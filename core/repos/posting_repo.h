#pragma once
#include <QHash>
#include <QMultiHash>
#include <QList>
#include <optional>
#include <QtGlobal>

#include "../models/posting.h"

class PostingRepo {
public:
    qint64 create(const Posting& p);
    bool remove(qint64 id);
    std::optional<Posting> get(qint64 id) const;

    QList<Posting> list() const;
    QList<Posting> list_by_invoice_line(qint64 invoice_line_id) const;

private:
    QHash<qint64, Posting> by_id;
    QMultiHash<qint64, qint64> by_invoice_line; // invoice_line_id -> posting_id
    qint64 next_id = 1;
};
