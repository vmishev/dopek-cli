#include "tool_service.h"

ToolService::ToolService(ToolRepo& tools) : tools_(tools) {}

Result<Id> ToolService::add(const QString& code, const QString& name, const QString& tool_class, const QString& notes) {
    Tool t;
    t.code = code.trimmed();
    t.name = name.trimmed();
    t.tool_class = tool_class.trimmed();
    t.notes = notes.trimmed();
    return Result<Id>::success(tools_.create(t));
}

ResultVoid ToolService::edit(Id id, const QString& code, const QString& name, const QString& tool_class, const QString& notes) {
    auto ex = tools_.get(id);
    if (!ex) return ResultVoid::failure("tool not found");
    Tool t = *ex;
    if (!code.trimmed().isEmpty()) t.code = code.trimmed();
    if (!name.trimmed().isEmpty()) t.name = name.trimmed();
    if (!tool_class.trimmed().isEmpty()) t.tool_class = tool_class.trimmed();
    if (!notes.isNull()) t.notes = notes.trimmed();
    if (!tools_.update(id, t)) return ResultVoid::failure("update failed");
    return ResultVoid::success();
}

std::optional<Tool> ToolService::get(Id id) const { return tools_.get(id); }
QList<Tool> ToolService::list() const { return tools_.list(); }
ResultVoid ToolService::remove(Id id) {
    if (!tools_.remove(id)) return ResultVoid::failure("tool not found");
    return ResultVoid::success();
}
