#pragma once
#include <QString>
#include <QList>
#include <optional>
#include <QtGlobal>

#include "result.h"
#include "../models/counterparty.h"
#include "../repos/counterparty_repo.h"

class CounterpartyService {
public:
    explicit CounterpartyService(CounterpartyRepo& repo);

    Result<qint64> add(const QString& name,
                       const QString& eik,
                       bool vat_reg,
                       const QString& mol,
                       const QString& address,
                       const QString& email);

    // patch-style: only provided fields are changed
    ResultVoid edit(qint64 id,
                    const std::optional<QString>& name,
                    const std::optional<QString>& eik,
                    const std::optional<bool>& vat_reg,
                    const std::optional<QString>& mol,
                    const std::optional<QString>& address,
                    const std::optional<QString>& email);

    Result<Counterparty> get(qint64 id) const;
    Result<QList<Counterparty>> list() const;

private:
    CounterpartyRepo& repo;

    static bool isBlank(const QString& s);
    static bool isEik9(const QString& s);
    static bool isEmailBasic(const QString& s);
};
