#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include <QtGlobal>

#include "../models/counterparty.h"

class CounterpartyRepo {
public:
    qint64 create(const Counterparty& c);
    bool update(qint64 id, const Counterparty& c);
    std::optional<Counterparty> get(qint64 id) const;
    bool remove(qint64 id);
    QList<Counterparty> list() const;

private:
    QHash<qint64, Counterparty> data;
    qint64 next_id = 1;
};
