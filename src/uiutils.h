#pragma once
#include <QString>

namespace UIUtils
{
    extern QString g_defaultStyle;

    QString formattedValue(const QString& value, const QString& color);
    QString formattedKeyValue(const QString& key, const QString& keyColor, const QString& value, const QString& valueColor);
    void setAppStyle(const QString& styleName, bool dark);
}
