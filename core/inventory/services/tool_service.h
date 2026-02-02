#pragma once
#include "../../common/services/result.h"
#include "../repos/tool_repo.h"

class ToolService {
public:
    explicit ToolService(ToolRepo& tools);

    Result<Id> add(const QString& code, const QString& name, const QString& tool_class, const QString& notes);
    ResultVoid edit(Id id, const QString& code, const QString& name, const QString& tool_class, const QString& notes);
    std::optional<Tool> get(Id id) const;
    QList<Tool> list() const;
    ResultVoid remove(Id id);

private:
    ToolRepo& tools_;
};
