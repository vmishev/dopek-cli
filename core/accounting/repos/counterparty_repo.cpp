#include "counterparty_repo.h"

qint64 CounterpartyRepo::create(const Counterparty& c) {
    Counterparty copy = c;
    copy.id = next_id;
    data.insert(next_id, copy);
    return next_id++;
}

bool CounterpartyRepo::update(qint64 id, const Counterparty& c) {
    if (!data.contains(id)) return false;
    Counterparty copy = c;
    copy.id = id;
    data[id] = copy;
    return true;
}

std::optional<Counterparty> CounterpartyRepo::get(qint64 id) const {
    if (!data.contains(id)) return std::nullopt;
    return data.value(id);
}

bool CounterpartyRepo::remove(qint64 id) {
    return data.remove(id) > 0;
}

QList<Counterparty> CounterpartyRepo::list() const {
    return data.values();
}
