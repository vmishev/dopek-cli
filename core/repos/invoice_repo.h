#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include <QtGlobal>

#include "../models/invoice.h"

class InvoiceRepo {
public:
    qint64 create(const Invoice& inv);
    bool update(qint64 id, const Invoice& inv);
    std::optional<Invoice> get(qint64 id) const;
    bool remove(qint64 id);
    QList<Invoice> list() const;

private:
    QHash<qint64, Invoice> data;
    qint64 next_id = 1;
};
