// core/models/material.h
#pragma once
#include <QString>
#include <QtGlobal>

struct Material {
    qint64 id = 0;
    QString name;
    QString spec;
    QString unit;
};
