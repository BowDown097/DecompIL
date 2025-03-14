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

    csImplicitUsings = settings.value("cs/implicitUsings", false).toBool();
    csStripILWarnings = settings.value("cs/stripILWarnings", true).toBool();

    editorFont = settings.value("editor/font", QFontDatabase::systemFont(QFontDatabase::FixedFont).toString()).toString();
    editorShowScrollBarH = settings.value("editor/showScrollBarH", true).toBool();
    editorShowScrollBarV = settings.value("editor/showScrollBarV", true).toBool();
    editorShowSidebar = settings.value("editor/showSidebar", true).toBool();
    editorTabWidth = settings.value("editor/tabWidth", 4).toInt();
    editorTheme = settings.value("editor/theme", CodeEditorDefinitions::defaultTheme().translatedName()).toString();
}

void SettingsStore::save()
{
    QSettings settings(configPath, QSettings::IniFormat);

    settings.setValue("appStyle", appStyle);
    settings.setValue("darkTheme", darkTheme);

    settings.setValue("cs/implicitUsings", csImplicitUsings);
    settings.setValue("cs/stripILWarnings", csStripILWarnings);

    settings.setValue("editor/font", editorFont);
    settings.setValue("editor/showScrollBarH", editorShowScrollBarH);
    settings.setValue("editor/showScrollBarV", editorShowScrollBarV);
    settings.setValue("editor/showSidebar", editorShowSidebar);
    settings.setValue("editor/tabWidth", editorTabWidth);
    settings.setValue("editor/theme", editorTheme);
}
