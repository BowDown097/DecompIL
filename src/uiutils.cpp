#include "uiutils.h"
#include <QApplication>
#include <QStyle>
#include <QStyleFactory>

namespace UIUtils
{
    QString g_defaultStyle;

    QString formattedValue(const QString& value, const QString& color)
    {
        return QStringLiteral("<font color='%1'>%2</font>").arg(color, value);
    }

    QString formattedKeyValue(const QString& key, const QString& keyColor, const QString& value, const QString& valueColor)
    {
        return formattedValue(key, keyColor) + '=' + formattedValue(value, valueColor);
    }

    void setAppStyle(const QString& styleName, bool dark)
    {
        if (styleName == "Default" && qApp->style()->objectName() != g_defaultStyle)
            qApp->setStyle(QStyleFactory::create(g_defaultStyle));
        else if (QStyle* style = QStyleFactory::create(styleName))
            qApp->setStyle(style);

        if (dark)
        {
            QPalette darkPalette;
            darkPalette.setColor(QPalette::Window, QColor(49,49,49));
            darkPalette.setColor(QPalette::WindowText, Qt::white);
            darkPalette.setColor(QPalette::Base, QColor(34,34,34));
            darkPalette.setColor(QPalette::AlternateBase, QColor(42,42,42));
            darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
            darkPalette.setColor(QPalette::ToolTipText, Qt::white);
            darkPalette.setColor(QPalette::Text, Qt::white);
            darkPalette.setColor(QPalette::Button, QColor(48,48,48));
            darkPalette.setColor(QPalette::ButtonText, Qt::white);
            darkPalette.setColor(QPalette::BrightText, Qt::red);
            darkPalette.setColor(QPalette::Link, QColor(47,163,198));
            darkPalette.setColor(QPalette::Highlight, QColor(145,205,92));
            darkPalette.setColor(QPalette::HighlightedText, Qt::black);
            darkPalette.setColor(QPalette::PlaceholderText, Qt::darkGray);
            qApp->setPalette(darkPalette);
        }
    }
}
