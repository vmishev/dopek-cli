#include "invoice_service.h"

InvoiceService::InvoiceService(InvoiceRepo& inv, CounterpartyRepo& cp, InvoiceLineRepo& ln)
    : invoices(inv), counterparties(cp), lines(ln) {}

bool InvoiceService::isBlank(const QString& s) {
    return s.trimmed().isEmpty();
}

bool InvoiceService::isDirectionValid(const QString& s) {
    const QString v = s.trimmed();
    return (v == "sale" || v == "purchase");
}

Result<qint64> InvoiceService::add(const QString& number, qint64 counterparty_id,
                                   const QDate& date, const QString& direction) {
    if (isBlank(number)) return Result<qint64>::failure("invoice.number is required");
    if (!counterparties.get(counterparty_id).has_value())
        return Result<qint64>::failure("invoice.counterparty_id not found");
    if (!date.isValid()) return Result<qint64>::failure("invoice.date is invalid");
    if (!isDirectionValid(direction)) return Result<qint64>::failure("invoice.direction invalid");

    Invoice inv;
    inv.number = number.trimmed();
    inv.counterparty_id = counterparty_id;
    inv.date = date;
    inv.direction = direction.trimmed();
    inv.net_amount = 0.0;

    return Result<qint64>::success(invoices.create(inv));
}

ResultVoid InvoiceService::update(qint64 id, const QString& number, qint64 counterparty_id,
                                  const QDate& date, const QString& direction) {
    if (!invoices.get(id).has_value()) return ResultVoid::failure("invoice not found");
    if (isBlank(number)) return ResultVoid::failure("invoice.number is required");
    if (!counterparties.get(counterparty_id).has_value())
        return ResultVoid::failure("invoice.counterparty_id not found");
    if (!date.isValid()) return ResultVoid::failure("invoice.date is invalid");
    if (!isDirectionValid(direction)) return ResultVoid::failure("invoice.direction invalid");

    Invoice inv;
    inv.id = id;
    inv.number = number.trimmed();
    inv.counterparty_id = counterparty_id;
    inv.date = date;
    inv.direction = direction.trimmed();

    // keep current net_amount; can be recalculated separately
    auto existing = invoices.get(id).value();
    inv.net_amount = existing.net_amount;

    if (!invoices.update(id, inv)) return ResultVoid::failure("invoice update failed");
    return ResultVoid::success();
}

Result<Invoice> InvoiceService::get(qint64 id) const {
    auto inv = invoices.get(id);
    if (!inv.has_value()) return Result<Invoice>::failure("invoice not found");
    return Result<Invoice>::success(inv.value());
}

Result<QList<Invoice>> InvoiceService::list() const {
    return Result<QList<Invoice>>::success(invoices.list());
}

ResultVoid InvoiceService::edit(qint64 id,
                                const std::optional<QString>& number,
                                const std::optional<qint64>& counterparty_id,
                                const std::optional<QDate>& date,
                                const std::optional<QString>& direction) {
    auto old = invoices.get(id);
    if (!old.has_value()) return ResultVoid::failure("invoice not found");

    Invoice updated = old.value();

    if (number.has_value()) {
        if (number.value().trimmed().isEmpty()) return ResultVoid::failure("invoice.number cannot be blank");
        updated.number = number.value().trimmed();
    }

    if (counterparty_id.has_value()) {
        // validate counterparty exists (same logic as add)
        auto cp = counterparties.get(counterparty_id.value());
        if (!cp.has_value()) return ResultVoid::failure("counterparty not found");
        updated.counterparty_id = counterparty_id.value();
    }

    if (date.has_value()) {
        if (!date.value().isValid()) return ResultVoid::failure("invoice.date invalid");
        updated.date = date.value();
    }

    if (direction.has_value()) {
        const QString d = direction.value().trimmed();
        if (d != "purchase" && d != "sale") return ResultVoid::failure("invoice.dir must be purchase|sale");
        updated.direction = d;
    }

    if (!invoices.update(id, updated)) return ResultVoid::failure("invoice update failed");
    return ResultVoid::success();
}
ResultVoid InvoiceService::recalc_net_amount(qint64 invoice_id) {
    auto inv = invoices.get(invoice_id);
    if (!inv.has_value()) return ResultVoid::failure("invoice not found");

    const auto ls = lines.list_by_invoice(invoice_id);
    double sum = 0.0;
    for (const auto& l : ls) sum += l.net_amount;

    Invoice updated = inv.value();
    updated.net_amount = sum;

    if (!invoices.update(invoice_id, updated))
        return ResultVoid::failure("invoice recalc update failed");

    return ResultVoid::success();
}
