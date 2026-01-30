#include "invoice_line_repo.h"

qint64 InvoiceLineRepo::create(const InvoiceLine& line) {
    InvoiceLine copy = line;
    copy.id = next_id;

    by_id.insert(next_id, copy);
    by_invoice.insert(copy.invoice_id, next_id);

    return next_id++;
}

bool InvoiceLineRepo::update(qint64 id, const InvoiceLine& line) {
    if (!by_id.contains(id)) return false;

    const qint64 old_invoice_id = by_id.value(id).invoice_id;

    InvoiceLine copy = line;
    copy.id = id;
    by_id[id] = copy;

    if (old_invoice_id != copy.invoice_id)
        reindex_line(id, old_invoice_id, copy.invoice_id);

    return true;
}

std::optional<InvoiceLine> InvoiceLineRepo::get(qint64 id) const {
    if (!by_id.contains(id)) return std::nullopt;
    return by_id.value(id);
}

bool InvoiceLineRepo::remove(qint64 id) {
    if (!by_id.contains(id)) return false;

    const qint64 invoice_id = by_id.value(id).invoice_id;
    by_id.remove(id);

    // remove one (invoice_id -> id) pair
    auto it = by_invoice.find(invoice_id);
    while (it != by_invoice.end() && it.key() == invoice_id) {
        if (it.value() == id) { it = by_invoice.erase(it); break; }
        ++it;
    }
    return true;
}

QList<InvoiceLine> InvoiceLineRepo::list() const {
    return by_id.values();
}

QList<InvoiceLine> InvoiceLineRepo::list_by_invoice(qint64 invoice_id) const {
    QList<InvoiceLine> out;
    const auto ids = by_invoice.values(invoice_id);
    out.reserve(ids.size());
    for (qint64 id : ids) {
        if (by_id.contains(id)) out.push_back(by_id.value(id));
    }
    return out;
}

void InvoiceLineRepo::reindex_line(qint64 line_id, qint64 old_invoice_id, qint64 new_invoice_id) {
    // remove old mapping
    auto it = by_invoice.find(old_invoice_id);
    while (it != by_invoice.end() && it.key() == old_invoice_id) {
        if (it.value() == line_id) { it = by_invoice.erase(it); break; }
        ++it;
    }
    // add new mapping
    by_invoice.insert(new_invoice_id, line_id);
}
