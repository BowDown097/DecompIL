#include "settingsform.h"
#include "ui_settingsform.h"
#include "decompilapplication.h"
#include "mainwindow.h"
#include "utils/uiutils.h"
#include "widgets/codeeditor/codeeditor.h"
#include "widgets/codeeditor/codeeditordefinitions.h"
#include "widgets/codeeditor/codeeditorsidebar.h"
#include "widgets/findbar.h"
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Theme>
#include <QFontDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QStyleFactory>

SettingsForm::SettingsForm(QWidget* parent)
    : QWidget(parent), ui(new Ui::SettingsForm)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    fillFromSettings();

    connect(ui->appStyle, &QComboBox::currentIndexChanged, this, &SettingsForm::changeAppStyle);
    connect(ui->darkTheme, &QCheckBox::clicked, this, &SettingsForm::changeAppStyle);
    connect(ui->editorFontButton, &QToolButton::clicked, this, &SettingsForm::promptFontChange);
    connect(ui->editorTabWidthSpin, &QSpinBox::valueChanged, this, &SettingsForm::changeEditorTabWidth);
    connect(ui->editorThemeCombo, &QComboBox::currentIndexChanged, this, &SettingsForm::changeEditorTheme);
    connect(ui->saveButton, &QPushButton::clicked, this, &SettingsForm::saveSettings);
    connect(ui->showScrollBarH, &QCheckBox::clicked, this, &SettingsForm::toggleEditorScrollBarH);
    connect(ui->showScrollBarV, &QCheckBox::clicked, this, &SettingsForm::toggleEditorScrollBarV);
    connect(ui->showSidebar, &QCheckBox::clicked, this, &SettingsForm::toggleEditorSidebar);

    enableSaveFor<QToolButton>(&QToolButton::clicked);
    enableSaveFor<QCheckBox>(&QCheckBox::clicked);
    enableSaveFor<QSpinBox>(&QSpinBox::valueChanged);
    enableSaveFor<QComboBox>(&QComboBox::currentIndexChanged);
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::changeAppStyle()
{
    UIUtils::setAppStyle(ui->appStyle->currentText(), ui->darkTheme->isChecked());
}

void SettingsForm::changeEditorTabWidth(int width)
{
    MainWindow::codeEditor()->setTabWidth(width);
}

void SettingsForm::changeEditorTheme(int index)
{
    MainWindow::codeEditor()->setTheme(CodeEditorDefinitions::themes().at(index));
}

void SettingsForm::closeEvent(QCloseEvent* event)
{
    if (ui->saveButton->isEnabled())
    {
        QMessageBox::StandardButton unsavedResponse = QMessageBox::warning(this,
            tr("Unsaved changes"), tr("You have unsaved changes! Would you like to save them?"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        if (unsavedResponse == QMessageBox::Yes)
            saveSettings();
        else
            revertLiveChanges();
    }

    QWidget::closeEvent(event);
}

void SettingsForm::enableSaveButton()
{
    ui->saveButton->setEnabled(true);
}

void SettingsForm::fillFromSettings()
{
    SettingsStore& store = decompILApp->settings();

    ui->appStyle->addItems(QStyleFactory::keys());
    ui->appStyle->setCurrentIndex(ui->appStyle->findText(store.appStyle));
    ui->darkTheme->setChecked(store.darkTheme);

    ui->implicitUsings->setChecked(store.csImplicitUsings);
    ui->stripILWarnings->setChecked(store.csStripILWarnings);

    ui->showScrollBarH->setChecked(store.editorShowScrollBarH);
    ui->showScrollBarV->setChecked(store.editorShowScrollBarV);
    ui->showSidebar->setChecked(store.editorShowSidebar);

    QFont font;
    font.fromString(store.editorFont);
    ui->editorFontDisplayLabel->setText(font.family() + ' ' + QString::number(font.pointSize()));
    ui->editorFontDisplayLabel->setFont(font);

    const QList<KSyntaxHighlighting::Theme> themes = CodeEditorDefinitions::themes();
    for (const KSyntaxHighlighting::Theme& theme : themes)
        ui->editorThemeCombo->addItem(theme.translatedName(), theme.name());
    ui->editorThemeCombo->setCurrentIndex(ui->editorThemeCombo->findText(store.editorTheme));

    ui->editorTabWidthSpin->setValue(qMin(store.editorTabWidth, ui->editorTabWidthSpin->maximum()));
}

void SettingsForm::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(event);
}

void SettingsForm::promptFontChange()
{
    bool ok{};
    QFont font = QFontDialog::getFont(&ok, ui->editorFontDisplayLabel->font(), this);
    if (ok)
    {
        MainWindow::codeEditor()->setFont(font);
        ui->editorFontDisplayLabel->setFont(font);
        ui->editorFontDisplayLabel->setText(font.family() + ' ' + QString::number(font.pointSize()));
    }
}

void SettingsForm::revertLiveChanges()
{
    SettingsStore& store = decompILApp->settings();
    UIUtils::setAppStyle(store.appStyle, store.darkTheme);

    QFont font;
    font.fromString(store.editorFont);
    MainWindow::codeEditor()->setFont(font);

    toggleEditorSidebar(store.editorShowSidebar);
    toggleEditorScrollBarH(store.editorShowScrollBarH);
    toggleEditorScrollBarV(store.editorShowScrollBarV);
    changeEditorTabWidth(store.editorTabWidth);
    changeEditorTheme(ui->editorThemeCombo->findText(store.editorTheme));
}

void SettingsForm::saveSettings()
{
    ui->saveButton->setEnabled(false);

    SettingsStore& store = decompILApp->settings();

    store.appStyle = ui->appStyle->currentText();
    store.darkTheme = ui->darkTheme->isChecked();

    store.csImplicitUsings = ui->implicitUsings->isChecked();
    store.csStripILWarnings = ui->stripILWarnings->isChecked();

    store.editorFont = ui->editorFontDisplayLabel->font().toString();
    store.editorShowScrollBarH = ui->showScrollBarH->isChecked();
    store.editorShowScrollBarV = ui->showScrollBarV->isChecked();
    store.editorShowSidebar = ui->showSidebar->isChecked();
    store.editorTabWidth = ui->editorTabWidthSpin->value();
    store.editorTheme = ui->editorThemeCombo->currentData().toString();

    store.save();
    store.initialize();

    QMessageBox::information(this, tr("Saved!"), tr("Settings saved successfully."));
}

void SettingsForm::toggleEditorScrollBarH(bool on)
{
    MainWindow::codeEditor()->setHorizontalScrollBarPolicy(on ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
}

void SettingsForm::toggleEditorScrollBarV(bool on)
{
    MainWindow::codeEditor()->setVerticalScrollBarPolicy(on ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
    MainWindow::codeEditor()->findBar()->updatePosition();
}

void SettingsForm::toggleEditorSidebar(bool on)
{
    MainWindow::codeEditor()->sidebar()->setVisible(on);
    MainWindow::codeEditor()->updateSidebarGeometry();
}
