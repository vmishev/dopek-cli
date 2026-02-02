#include "ledger_event_repo.h"

Id LedgerEventRepo::create(const LedgerEvent& obj) {
    LedgerEvent e = obj; e.id = nextId_++;
    data_.insert(e.id, e);
    return e.id;
}

std::optional<LedgerEvent> LedgerEventRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}

bool LedgerEventRepo::update(Id id, const LedgerEvent& obj) {
    if (!data_.contains(id)) return false;
    LedgerEvent e = obj; e.id = id;
    data_.insert(id, e);
    return true;
}

bool LedgerEventRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<LedgerEvent> LedgerEventRepo::list() const { return data_.values(); }

QList<LedgerEvent> LedgerEventRepo::list_by_type(const QString& type) const {
    QList<LedgerEvent> out;
    for (const auto& v : data_) if (v.type == type) out.append(v);
    return out;
}
