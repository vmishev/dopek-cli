#pragma once
#include <QString>
#include <QtGlobal>

struct Posting {
    qint64 id = 0;
    qint64 invoice_line_id = 0;
    QString posting_type;     // "project" | "inventory"
    qint64 project_id = 0;    // optional
    qint64 material_id = 0;   // optional
};
