#include "counterparty_service.h"

CounterpartyService::CounterpartyService(CounterpartyRepo& r) : repo(r) {}

bool CounterpartyService::isBlank(const QString& s) {
    return s.trimmed().isEmpty();
}

Result<qint64> CounterpartyService::add(const QString& name, const QString& vat_id,
                                        const QString& country, const QString& address) {
    if (isBlank(name)) return Result<qint64>::fail("counterparty.name is required");

    Counterparty c;
    c.name = name.trimmed();
    c.vat_id = vat_id.trimmed();
    c.country = country.trimmed();
    c.address = address.trimmed();

    return Result<qint64>::success(repo.create(c));
}

ResultVoid CounterpartyService::update(qint64 id, const QString& name, const QString& vat_id,
                                       const QString& country, const QString& address) {
    if (!repo.get(id).has_value()) return ResultVoid::fail("counterparty not found");
    if (isBlank(name)) return ResultVoid::fail("counterparty.name is required");

    Counterparty c;
    c.id = id;
    c.name = name.trimmed();
    c.vat_id = vat_id.trimmed();
    c.country = country.trimmed();
    c.address = address.trimmed();

    if (!repo.update(id, c)) return ResultVoid::fail("counterparty update failed");
    return ResultVoid::success();
}

Result<Counterparty> CounterpartyService::get(qint64 id) const {
    auto c = repo.get(id);
    if (!c.has_value()) return Result<Counterparty>::fail("counterparty not found");
    return Result<Counterparty>::success(c.value());
}

Result<QList<Counterparty>> CounterpartyService::list() const {
    return Result<QList<Counterparty>>::success(repo.list());
}
