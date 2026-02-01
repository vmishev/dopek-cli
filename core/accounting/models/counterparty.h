#pragma once
#include <QString>
#include <QtGlobal>

struct Counterparty {
    qint64 id = 0;

    QString name;
    QString eik;          // 9 digits as string
    bool vat_reg = false;

    QString vat_id;
    QString country;

    QString mol;          // materialno otgovorno lice (as you define it)
    QString address;
    QString email;
};
