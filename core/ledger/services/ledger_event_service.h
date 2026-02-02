#pragma once
#include <QDateTime>
#include "../../common/services/result.h"
#include "../repos/ledger_event_repo.h"

class LedgerEventService {
public:
    explicit LedgerEventService(LedgerEventRepo& events);

    Result<Id> add(const QString& type, const QString& payload_json, const QString& refs_json, const QDateTime& ts);
    ResultVoid edit(Id id, const QString& type, const QString& payload_json, const QString& refs_json);
    std::optional<LedgerEvent> get(Id id) const;
    QList<LedgerEvent> list() const;
    QList<LedgerEvent> list_by_type(const QString& type) const;
    ResultVoid remove(Id id);

private:
    LedgerEventRepo& events_;
};
