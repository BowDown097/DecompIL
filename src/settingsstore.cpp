#include "settingsstore.h"
#include "ui/widgets/codeeditor/codeeditordefinitions.h"
#include <KSyntaxHighlighting/Theme>
#include <QFontDatabase>
#include <QSettings>

void SettingsStore::initialize()
{
    QSettings settings(configPath, QSettings::IniFormat);

    appStyle = settings.value("appStyle", "Default").toString();
    darkTheme = settings.value("darkTheme", false).toBool();
    implicitUsings = settings.value("implicitUsings", false).toBool();
    stripILWarnings = settings.value("stripILWarnings", true).toBool();

    editorFont = settings.value("editor/font", QFontDatabase::systemFont(QFontDatabase::FixedFont).toString()).toString();
    editorTabWidth = settings.value("editor/tabWidth", 4).toInt();
    editorTheme = settings.value("editor/theme", CodeEditorDefinitions::defaultTheme().translatedName()).toString();
}

void SettingsStore::save()
{
    QSettings settings(configPath, QSettings::IniFormat);

    settings.setValue("appStyle", appStyle);
    settings.setValue("darkTheme", darkTheme);
    settings.setValue("implicitUsings", implicitUsings);
    settings.setValue("stripILWarnings", stripILWarnings);

    settings.setValue("editor/font", editorFont);
    settings.setValue("editor/tabWidth", editorTabWidth);
    settings.setValue("editor/theme", editorTheme);
}
