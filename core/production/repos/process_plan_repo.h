#pragma once
#include <QHash>
#include <QList>
#include <optional>
#include "../models/process_plan.h"

class ProcessPlanRepo {
public:
    Id create(const ProcessPlan& obj);
    std::optional<ProcessPlan> get(Id id) const;
    bool update(Id id, const ProcessPlan& obj);
    bool remove(Id id);
    QList<ProcessPlan> list() const;
    QList<ProcessPlan> list_by_part(Id part_id) const;

private:
    QHash<Id, ProcessPlan> data_;
    Id nextId_ = 1;
};
