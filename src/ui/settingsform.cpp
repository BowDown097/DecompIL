#include "settingsform.h"
#include "ui_settingsform.h"
#include "decompilapplication.h"
#include "mainwindow.h"
#include "utils/uiutils.h"
#include "widgets/codeeditor/codeeditor.h"
#include "widgets/codeeditor/codeeditordefinitions.h"
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

    for (QToolButton* toolButton : findChildren<QToolButton*>())
        connect(toolButton, &QToolButton::clicked, this, &SettingsForm::enableSaveButton);
    for (QCheckBox* checkBox : findChildren<QCheckBox*>())
        connect(checkBox, &QCheckBox::clicked, this, &SettingsForm::enableSaveButton);
    for (QSpinBox* spinBox : findChildren<QSpinBox*>())
        connect(spinBox, &QSpinBox::valueChanged, this, &SettingsForm::enableSaveButton);
    for (QComboBox* comboBox : findChildren<QComboBox*>())
        connect(comboBox, &QComboBox::currentIndexChanged, this, &SettingsForm::enableSaveButton);
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
    ui->implicitUsings->setChecked(store.implicitUsings);
    ui->stripILWarnings->setChecked(store.stripILWarnings);

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

    MainWindow::codeEditor()->setTabWidth(store.editorTabWidth);
    MainWindow::codeEditor()->setTheme(CodeEditorDefinitions::themes().at(ui->editorThemeCombo->findText(store.editorTheme)));
}

void SettingsForm::saveSettings()
{
    ui->saveButton->setEnabled(false);

    SettingsStore& store = decompILApp->settings();
    store.appStyle = ui->appStyle->currentText();
    store.darkTheme = ui->darkTheme->isChecked();
    store.editorFont = ui->editorFontDisplayLabel->font().toString();
    store.editorTabWidth = ui->editorTabWidthSpin->value();
    store.editorTheme = ui->editorThemeCombo->currentData().toString();
    store.implicitUsings = ui->implicitUsings->isChecked();
    store.stripILWarnings = ui->stripILWarnings->isChecked();

    store.save();
    store.initialize();

    QMessageBox::information(this, tr("Saved!"), tr("Settings saved successfully."));
}
