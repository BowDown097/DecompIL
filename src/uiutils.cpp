#include "uiutils.h"

namespace UIUtils
{
    QString formattedValue(const QString& value, const QString& color)
    {
        return QStringLiteral("<font color='%1'>%2</font>").arg(color, value);
    }

    QString formattedKeyValue(const QString& key, const QString& keyColor, const QString& value, const QString& valueColor)
    {
        return formattedValue(key, keyColor) + '=' + formattedValue(value, valueColor);
    }
}
