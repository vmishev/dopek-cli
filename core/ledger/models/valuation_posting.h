#pragma once
#include <QString>
#include <QDate>
#include "../../common/util/ids.h"

struct ValuationPosting {
    Id id = 0;
    QDate date;
    QString description;
    QString debit_account;
    QString credit_account;
    double amount = 0.0;
    QString refs_json;     // link to events/invoices/projects
};
