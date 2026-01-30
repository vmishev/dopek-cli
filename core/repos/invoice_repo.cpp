#include "invoice_repo.h"

qint64 InvoiceRepo::create(const Invoice& inv) {
    Invoice copy = inv;
    copy.id = next_id;
    data.insert(next_id, copy);
    return next_id++;
}

bool InvoiceRepo::update(qint64 id, const Invoice& inv) {
    if (!data.contains(id)) return false;
    Invoice copy = inv;
    copy.id = id;
    data[id] = copy;
    return true;
}

std::optional<Invoice> InvoiceRepo::get(qint64 id) const {
    if (!data.contains(id)) return std::nullopt;
    return data.value(id);
}

bool InvoiceRepo::remove(qint64 id) {
    return data.remove(id) > 0;
}

QList<Invoice> InvoiceRepo::list() const {
    return data.values();
}
