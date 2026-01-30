#include "posting_repo.h"

qint64 PostingRepo::create(const Posting& p) {
    Posting copy = p;
    copy.id = next_id;

    by_id.insert(next_id, copy);
    by_invoice_line.insert(copy.invoice_line_id, next_id);

    return next_id++;
}

bool PostingRepo::remove(qint64 id) {
    if (!by_id.contains(id)) return false;

    const qint64 il_id = by_id.value(id).invoice_line_id;
    by_id.remove(id);

    auto it = by_invoice_line.find(il_id);
    while (it != by_invoice_line.end() && it.key() == il_id) {
        if (it.value() == id) { it = by_invoice_line.erase(it); break; }
        ++it;
    }
    return true;
}

std::optional<Posting> PostingRepo::get(qint64 id) const {
    if (!by_id.contains(id)) return std::nullopt;
    return by_id.value(id);
}

QList<Posting> PostingRepo::list() const {
    return by_id.values();
}

QList<Posting> PostingRepo::list_by_invoice_line(qint64 invoice_line_id) const {
    QList<Posting> out;
    const auto ids = by_invoice_line.values(invoice_line_id);
    out.reserve(ids.size());
    for (qint64 id : ids) {
        if (by_id.contains(id)) out.push_back(by_id.value(id));
    }
    return out;
}
