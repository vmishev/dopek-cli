#pragma once
#include <QString>
#include <QDate>
#include <QtGlobal>

struct Invoice {
    qint64 id = 0;
    QString number;
    qint64 counterparty_id = 0;
    QDate date;
    QString direction;   // "sale" | "purchase"
    double net_amount = 0.0; // derived
};
