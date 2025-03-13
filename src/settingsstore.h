#pragma once
#include <QDir>
#include <QStandardPaths>

class SettingsStore
{
public:
    static inline const QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
                                           + QDir::separator() + DECOMPIL_APP_NAME
                                           + QDir::separator() + "settings.ini";

    QString appStyle;
    bool darkTheme;
    QString editorFont;
    int editorTabWidth;
    QString editorTheme;
    bool implicitUsings;

    void initialize();
    void save();
};
