#include "invoice_line_service.h"

InvoiceLineService::InvoiceLineService(InvoiceLineRepo& l, InvoiceRepo& inv, InvoiceService& is)
    : lines(l), invoices(inv), invoiceService(is) {}

bool InvoiceLineService::isBlank(const QString& s) { return s.trimmed().isEmpty(); }

bool InvoiceLineService::isLineTypeValid(const QString& s) {
    const QString v = s.trimmed();
    return (v == "goods" || v == "services");
}

bool InvoiceLineService::isLineNoUnique(qint64 invoice_id, int line_no, qint64 except_line_id) const {
    const auto ls = lines.list_by_invoice(invoice_id);
    for (const auto& l : ls) {
        if (l.id == except_line_id) continue;
        if (l.line_no == line_no) return false;
    }
    return true;
}

Result<qint64> InvoiceLineService::add(qint64 invoice_id, int line_no, const QString& description,
                                       const QString& line_type, double qty,
                                       const QString& unit, double unit_price) {
    if (!invoices.get(invoice_id).has_value())
        return Result<qint64>::fail("invoice_line.invoice_id not found");
    if (line_no <= 0) return Result<qint64>::fail("invoice_line.line_no must be > 0");
    if (!isLineNoUnique(invoice_id, line_no))
        return Result<qint64>::fail("invoice_line.line_no must be unique per invoice");
    if (isBlank(description)) return Result<qint64>::fail("invoice_line.description is required");
    if (!isLineTypeValid(line_type)) return Result<qint64>::fail("invoice_line.line_type invalid");
    if (qty < 0.0) return Result<qint64>::fail("invoice_line.qty must be >= 0");
    if (unit_price < 0.0) return Result<qint64>::fail("invoice_line.unit_price must be >= 0");
    if (isBlank(unit)) return Result<qint64>::fail("invoice_line.unit is required");

    InvoiceLine l;
    l.invoice_id = invoice_id;
    l.line_no = line_no;
    l.description = description.trimmed();
    l.line_type = line_type.trimmed();
    l.qty = qty;
    l.unit = unit.trimmed();
    l.unit_price = unit_price;
    l.net_amount = qty * unit_price;

    const qint64 id = lines.create(l);
    auto r = invoiceService.recalc_net_amount(invoice_id);
    if (!r.ok) return Result<qint64>::fail(r.error);
    return Result<qint64>::success(id);
}

ResultVoid InvoiceLineService::update(qint64 id, const QString& description,
                                      const QString& line_type, double qty,
                                      const QString& unit, double unit_price) {
    auto existing = lines.get(id);
    if (!existing.has_value()) return ResultVoid::fail("invoice_line not found");

    InvoiceLine l = existing.value();

    if (isBlank(description)) return ResultVoid::fail("invoice_line.description is required");
    if (!isLineTypeValid(line_type)) return ResultVoid::fail("invoice_line.line_type invalid");
    if (qty < 0.0) return ResultVoid::fail("invoice_line.qty must be >= 0");
    if (unit_price < 0.0) return ResultVoid::fail("invoice_line.unit_price must be >= 0");
    if (isBlank(unit)) return ResultVoid::fail("invoice_line.unit is required");

    l.description = description.trimmed();
    l.line_type = line_type.trimmed();
    l.qty = qty;
    l.unit = unit.trimmed();
    l.unit_price = unit_price;
    l.net_amount = qty * unit_price;

    if (!lines.update(id, l)) return ResultVoid::fail("invoice_line update failed");

    auto r = invoiceService.recalc_net_amount(l.invoice_id);
    if (!r.ok) return ResultVoid::fail(r.error);

    return ResultVoid::success();
}

ResultVoid InvoiceLineService::remove(qint64 id) {
    auto existing = lines.get(id);
    if (!existing.has_value()) return ResultVoid::fail("invoice_line not found");
    const qint64 invoice_id = existing.value().invoice_id;

    if (!lines.remove(id)) return ResultVoid::fail("invoice_line remove failed");

    auto r = invoiceService.recalc_net_amount(invoice_id);
    if (!r.ok) return ResultVoid::fail(r.error);

    return ResultVoid::success();
}

Result<QList<InvoiceLine>> InvoiceLineService::list(qint64 invoice_id) const {
    if (!invoices.get(invoice_id).has_value())
        return Result<QList<InvoiceLine>>::fail("invoice not found");
    return Result<QList<InvoiceLine>>::success(lines.list_by_invoice(invoice_id));
}
