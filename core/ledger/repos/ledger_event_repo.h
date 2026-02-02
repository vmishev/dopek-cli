#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/ledger_event.h"

class LedgerEventRepo {
public:
    Id create(const LedgerEvent& obj);
    std::optional<LedgerEvent> get(Id id) const;
    bool update(Id id, const LedgerEvent& obj);
    bool remove(Id id);
    QList<LedgerEvent> list() const;

    QList<LedgerEvent> list_by_type(const QString& type) const;

private:
    QHash<Id, LedgerEvent> data_;
    Id nextId_ = 1;
};
