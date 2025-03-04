#pragma once
#include <QString>

namespace UIUtils
{
    QString formattedValue(const QString& value, const QString& color);
    QString formattedKeyValue(const QString& key, const QString& keyColor, const QString& value, const QString& valueColor);
}
