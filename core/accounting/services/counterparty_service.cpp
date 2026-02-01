#include "counterparty_service.h"

CounterpartyService::CounterpartyService(CounterpartyRepo& r) : repo(r) {}

bool CounterpartyService::isBlank(const QString& s) {
    return s.trimmed().isEmpty();
}

bool CounterpartyService::isEik9(const QString& s) {
    const QString v = s.trimmed();
    if (v.size() != 9) return false;
    for (QChar ch : v) if (!ch.isDigit()) return false;
    return true;
}

// minimal (not strict RFC). good enough for MVP.
bool CounterpartyService::isEmailBasic(const QString& s) {
    const QString v = s.trimmed();
    if (v.isEmpty()) return true;           // optional
    const int at = v.indexOf('@');
    if (at <= 0) return false;
    if (v.indexOf('@', at + 1) != -1) return false;
    const int dot = v.lastIndexOf('.');
    return dot > at + 1 && dot < v.size() - 1;
}

Result<qint64> CounterpartyService::add(const QString& name,
                                        const QString& eik,
                                        bool vat_reg,
                                        const QString& mol,
                                        const QString& address,
                                        const QString& email) {
    if (isBlank(name)) return Result<qint64>::fail("counterparty.name is required");
    if (!isEik9(eik)) return Result<qint64>::fail("counterparty.eik must be exactly 9 digits");
    if (!isEmailBasic(email)) return Result<qint64>::fail("counterparty.email invalid");
    if (isBlank(address)) return Result<qint64>::fail("counterparty.address is required");

    Counterparty c;
    c.name = name.trimmed();
    c.eik = eik.trimmed();
    c.vat_reg = vat_reg;
    c.mol = mol.trimmed();
    c.address = address.trimmed();
    c.email = email.trimmed();

    return Result<qint64>::success(repo.create(c));
}

ResultVoid CounterpartyService::edit(qint64 id,
                                     const std::optional<QString>& name,
                                     const std::optional<QString>& eik,
                                     const std::optional<bool>& vat_reg,
                                     const std::optional<QString>& mol,
                                     const std::optional<QString>& address,
                                     const std::optional<QString>& email) {
    auto old = repo.get(id);
    if (!old.has_value()) return ResultVoid::fail("counterparty not found");

    Counterparty updated = old.value();

    if (name.has_value()) {
        if (isBlank(name.value())) return ResultVoid::fail("counterparty.name cannot be blank");
        updated.name = name.value().trimmed();
    }
    if (eik.has_value()) {
        if (!isEik9(eik.value())) return ResultVoid::fail("counterparty.eik must be exactly 9 digits");
        updated.eik = eik.value().trimmed();
    }
    if (vat_reg.has_value()) {
        updated.vat_reg = vat_reg.value();
    }
    if (mol.has_value()) {
        updated.mol = mol.value().trimmed();
    }
    if (address.has_value()) {
        if (isBlank(address.value())) return ResultVoid::fail("counterparty.address cannot be blank");
        updated.address = address.value().trimmed();
    }
    if (email.has_value()) {
        if (!isEmailBasic(email.value())) return ResultVoid::fail("counterparty.email invalid");
        updated.email = email.value().trimmed();
    }

    if (!repo.update(id, updated)) return ResultVoid::fail("counterparty update failed");
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
