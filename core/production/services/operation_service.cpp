#include "operation_service.h"

OperationService::OperationService(OperationRepo& ops, ProcessPlanRepo& plans, OperationCodeRepo& codes)
    : ops_(ops), plans_(plans), codes_(codes) {}

Result<Id> OperationService::add(Id process_plan_id,
                                 const QString& operation_code,
                                 int sequence_no,
                                 const QString& params_json) {
    if (!plans_.get(process_plan_id)) return Result<Id>::failure("process plan not found");
    if (!codes_.get_by_code(operation_code)) return Result<Id>::failure("operation_code not found");

    Operation o;
    o.process_plan_id = process_plan_id;
    o.operation_code = operation_code;
    o.sequence_no = sequence_no;
    o.params_json = params_json;
    const Id id = ops_.create(o);
    return Result<Id>::success(id);
}

ResultVoid OperationService::edit(Id id,
                                  const QString& operation_code,
                                  std::optional<int> sequence_no,
                                  const QString& params_json) {
    auto ex = ops_.get(id);
    if (!ex) return ResultVoid::failure("operation not found");
    Operation o = *ex;

    if (!operation_code.trimmed().isEmpty()) {
        if (!codes_.get_by_code(operation_code)) return ResultVoid::failure("operation_code not found");
        o.operation_code = operation_code;
    }
    if (sequence_no.has_value()) o.sequence_no = *sequence_no;
    if (!params_json.isNull()) o.params_json = params_json;

    if (!ops_.update(id, o)) return ResultVoid::failure("update failed");
    return ResultVoid::success();
}

std::optional<Operation> OperationService::get(Id id) const { return ops_.get(id); }
QList<Operation> OperationService::list() const { return ops_.list(); }
QList<Operation> OperationService::list_by_process_plan(Id process_plan_id) const { return ops_.list_by_process_plan(process_plan_id); }
ResultVoid OperationService::remove(Id id) {
    if (!ops_.remove(id)) return ResultVoid::failure("operation not found");
    return ResultVoid::success();
}
