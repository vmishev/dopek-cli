#include "operation_code_repo.h"

Id OperationCodeRepo::create(const OperationCode& obj) {
    OperationCode x = obj; x.id = nextId_++;
    data_.insert(x.id, x);
    return x.id;
}
std::optional<OperationCode> OperationCodeRepo::get(Id id) const {
    if (!data_.contains(id)) return std::nullopt;
    return data_.value(id);
}
bool OperationCodeRepo::update(Id id, const OperationCode& obj) {
    if (!data_.contains(id)) return false;
    OperationCode x = obj; x.id = id;
    data_.insert(id, x);
    return true;
}
bool OperationCodeRepo::remove(Id id) { return data_.remove(id) > 0; }
QList<OperationCode> OperationCodeRepo::list() const { return data_.values(); }

std::optional<OperationCode> OperationCodeRepo::get_by_code(const QString& code) const {
    for (const auto& v : data_) if (v.code == code) return v;
    return std::nullopt;
}
