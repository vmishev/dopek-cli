#pragma once
#include <QString>
#include <QList>
#include <QtGlobal>

#include "../services/result.h"
#include "../models/counterparty.h"
#include "../repos/counterparty_repo.h"

class CounterpartyService {
public:
    explicit CounterpartyService(CounterpartyRepo& repo);

    Result<qint64> add(const QString& name, const QString& vat_id,
                       const QString& country, const QString& address);

    ResultVoid update(qint64 id, const QString& name, const QString& vat_id,
                      const QString& country, const QString& address);

    Result<Counterparty> get(qint64 id) const;
    Result<QList<Counterparty>> list() const;

private:
    CounterpartyRepo& repo;
    static bool isBlank(const QString& s);
};
