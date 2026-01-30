#pragma once
#include <QString>
#include <QList>
#include <QtGlobal>

#include "../services/result.h"
#include "../models/invoice_line.h"
#include "../repos/invoice_line_repo.h"
#include "../repos/invoice_repo.h"
#include "invoice_service.h"

class InvoiceLineService {
public:
    InvoiceLineService(InvoiceLineRepo& lines, InvoiceRepo& invoices, InvoiceService& invoiceService);

    Result<qint64> add(qint64 invoice_id,
                       int line_no,
                       const QString& description,
                       const QString& line_type,
                       double qty,
                       const QString& unit,
                       double unit_price);

    ResultVoid update(qint64 id, const QString& description,
                      const QString& line_type, double qty, const QString& unit, double unit_price);

    ResultVoid remove(qint64 id);
    Result<QList<InvoiceLine>> list(qint64 invoice_id) const;

private:
    InvoiceLineRepo& lines;
    InvoiceRepo& invoices;
    InvoiceService& invoiceService;

    static bool isBlank(const QString& s);
    static bool isLineTypeValid(const QString& s);
    bool isLineNoUnique(qint64 invoice_id, int line_no, qint64 except_line_id = 0) const;
};
