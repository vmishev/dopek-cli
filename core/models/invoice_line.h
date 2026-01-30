#pragma once
#include <QString>
#include <QtGlobal>

struct InvoiceLine {
    qint64 id = 0;
    qint64 invoice_id = 0;
    int line_no = 0;
    QString description;
    QString line_type;   // "goods" | "services"
    double qty = 0.0;
    QString unit; // ?? "pcs" | "kg"
    double unit_price = 0.0;
    double net_amount = 0.0; // derived
};
