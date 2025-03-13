#pragma once
#include <QString>

namespace StringUtils
{
    void applyImplicitUsings(QString& str);
    void stripILWarnings(QString& str);
    void trimFront(QString& str);
};
