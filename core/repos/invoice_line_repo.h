#pragma once
#include <QHash>
#include <QList>
#include <QMultiHash>
#include <optional>
#include <QtGlobal>

#include "../models/invoice_line.h"

class InvoiceLineRepo {
public:
    qint64 create(const InvoiceLine& line);
    bool update(qint64 id, const InvoiceLine& line);
    std::optional<InvoiceLine> get(qint64 id) const;
    bool remove(qint64 id);

    QList<InvoiceLine> list() const;
    QList<InvoiceLine> list_by_invoice(qint64 invoice_id) const;

private:
    QHash<qint64, InvoiceLine> by_id;
    QMultiHash<qint64, qint64> by_invoice; // invoice_id -> line_id
    qint64 next_id = 1;

    void reindex_line(qint64 line_id, qint64 old_invoice_id, qint64 new_invoice_id);
};
