#include "ledger_event_service.h"

LedgerEventService::LedgerEventService(LedgerEventRepo& events) : events_(events) {}

Result<Id> LedgerEventService::add(const QString& type,
                                   const QString& payload_json,
                                   const QString& refs_json,
                                   const QDateTime& ts) {
    LedgerEvent e;
    e.type = type.trimmed();
    e.payload_json = payload_json;
    e.refs_json = refs_json;
    e.ts = ts.isValid() ? ts : QDateTime::currentDateTime();
    return Result<Id>::success(events_.create(e));
}

ResultVoid LedgerEventService::edit(Id id, const QString& type, const QString& payload_json, const QString& refs_json) {
    auto ex = events_.get(id);
    if (!ex) return ResultVoid::failure("ledger event not found");
    LedgerEvent e = *ex;
    if (!type.trimmed().isEmpty()) e.type = type.trimmed();
    if (!payload_json.isNull()) e.payload_json = payload_json;
    if (!refs_json.isNull()) e.refs_json = refs_json;
    if (!events_.update(id, e)) return ResultVoid::failure("update failed");
    return ResultVoid::success();
}

std::optional<LedgerEvent> LedgerEventService::get(Id id) const { return events_.get(id); }
QList<LedgerEvent> LedgerEventService::list() const { return events_.list(); }
QList<LedgerEvent> LedgerEventService::list_by_type(const QString& type) const { return events_.list_by_type(type); }
ResultVoid LedgerEventService::remove(Id id) {
    if (!events_.remove(id)) return ResultVoid::failure("ledger event not found");
    return ResultVoid::success();
}
