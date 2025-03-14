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
    bool csImplicitUsings;
    bool csStripILWarnings;
    QString editorFont;
    bool editorShowScrollBarH;
    bool editorShowScrollBarV;
    bool editorShowSidebar;
    int editorTabWidth;
    QString editorTheme;

    void initialize();
    void save();
};
