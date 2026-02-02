#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/tool.h"

class ToolRepo {
public:
    Id create(const Tool& obj);
    std::optional<Tool> get(Id id) const;
    bool update(Id id, const Tool& obj);
    bool remove(Id id);
    QList<Tool> list() const;

private:
    QHash<Id, Tool> data_;
    Id nextId_ = 1;
};
