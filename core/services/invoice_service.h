#pragma once
#include <QString>
#include <QDate>
#include <QList>
#include <QtGlobal>
#include <optional>

#include "../services/result.h"
#include "../models/invoice.h"
#include "../repos/invoice_repo.h"
#include "../repos/counterparty_repo.h"
#include "../repos/invoice_line_repo.h"

class InvoiceService {
public:
    InvoiceService(InvoiceRepo& invoices,
                   CounterpartyRepo& counterparties,
                   InvoiceLineRepo& lines);

    Result<qint64> add(const QString& number, qint64 counterparty_id,
                       const QDate& date, const QString& direction);

    ResultVoid update(qint64 id, const QString& number, qint64 counterparty_id,
                      const QDate& date, const QString& direction);

    Result<Invoice> get(qint64 id) const;
    Result<QList<Invoice>> list() const;

    ResultVoid edit(qint64 id,
                    const std::optional<QString>& number,
                    const std::optional<qint64>& counterparty_id,
                    const std::optional<QDate>& date,
                    const std::optional<QString>& direction);

    ResultVoid recalc_net_amount(qint64 invoice_id);

private:
    InvoiceRepo& invoices;
    CounterpartyRepo& counterparties;
    InvoiceLineRepo& lines;

    static bool isBlank(const QString& s);
    static bool isDirectionValid(const QString& s);
};
