// core/models/project.h
#pragma once
#include <QString>
#include <QtGlobal>

struct Project {
    qint64 id = 0;
    QString code;
    QString name;
    QString notes;
};
