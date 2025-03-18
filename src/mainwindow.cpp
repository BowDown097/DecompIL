#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "decompilapplication.h"
#include "interface/interface.h"
#include "languagemapping.h"
#include "ui/settingsform.h"
#include "ui/widgets/findbar.h"
#include "utils/stringutils.h"
#include <QDesktopServices>
#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_codeEditor = ui->codeEditor;
    setWindowTitle(DECOMPIL_APP_NAME " v" DECOMPIL_VERSION_NAME);

    connect(ui->closeAllAction, &QAction::triggered, ui->treeWidget, &AssemblyTreeWidget::closeAssemblies);
    connect(ui->openAction, &QAction::triggered, this, &MainWindow::promptForAssemblies);
    connect(ui->reloadAllAction, &QAction::triggered, ui->treeWidget, &AssemblyTreeWidget::reloadAssemblies);
    connect(ui->sortAssembliesAction, &QAction::triggered, ui->treeWidget, &AssemblyTreeWidget::sortAssemblies);

    connect(ui->findAction, &QAction::triggered, this, &MainWindow::findInEditor);
    connect(ui->redoAction, &QAction::triggered, this, &MainWindow::redoInEditor);
    connect(ui->searchAssembliesAction, &QAction::triggered, this, &MainWindow::searchAssemblies);
    connect(ui->undoAction, &QAction::triggered, this, &MainWindow::undoInEditor);

    connect(ui->collapseTreeNodesAction, &QAction::triggered, this, &MainWindow::collapseTreeNodes);
    connect(ui->fullScreenAction, &QAction::triggered, this, &MainWindow::toggleFullScreen);
    connect(ui->optionsAction, &QAction::triggered, this, &MainWindow::openSettingsForm);
    connect(ui->wordWrapAction, &QAction::triggered, this, &MainWindow::toggleWordWrap);

    connect(ui->sourceCodeAction, &QAction::triggered, this, &MainWindow::goToRepo);
    connect(ui->aboutAction, &QAction::triggered, this, &MainWindow::openAboutWindow);

    connect(ui->csVersionCombo, &QComboBox::currentIndexChanged, this, &MainWindow::comboBoxChanged);
    connect(ui->languageCombo, &QComboBox::currentIndexChanged, this, &MainWindow::comboBoxChanged);
    connect(ui->treeWidget, &AssemblyTreeWidget::decompileRequested, this, &MainWindow::decompileType);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::promptForAssemblies()
{
    ui->treeWidget->openAssemblies(QFileDialog::getOpenFileNames(
        this, tr("Choose .NET executable(s)"), QString(),
        tr(".NET Executables (*.exe *.dll *.netmodule *.winmd);;All Files (*)")));
}

void MainWindow::findInEditor()
{
    m_codeEditor->findBar()->setReveal(m_codeEditor->findBar()->isHidden());
}

void MainWindow::redoInEditor()
{
    ui->codeEditor->redo();
}

void MainWindow::searchAssemblies()
{

}

void MainWindow::undoInEditor()
{
    ui->codeEditor->undo();
}

void MainWindow::collapseTreeNodes()
{
    ui->treeWidget->collapseAll();
}

void MainWindow::openSettingsForm()
{
    SettingsForm* form = new SettingsForm;
    form->show();
}

void MainWindow::toggleFullScreen()
{
    if (windowState() == Qt::WindowFullScreen)
        showNormal();
    else
        showFullScreen();
}

void MainWindow::toggleWordWrap()
{
    ui->codeEditor->setWordWrapMode(ui->codeEditor->wordWrapMode() == QTextOption::WrapAtWordBoundaryOrAnywhere
        ? QTextOption::NoWrap : QTextOption::WrapAtWordBoundaryOrAnywhere);
}

void MainWindow::goToRepo()
{
    QDesktopServices::openUrl(QUrl(DECOMPIL_REPO_URL));
}

void MainWindow::openAboutWindow()
{

}

void MainWindow::comboBoxChanged(int)
{
    ui->csVersionCombo->setVisible(ui->languageCombo->currentText() == "C#");
    if (m_currentDecomp.handle)
        decompileType(m_currentDecomp.handle, m_currentDecomp.assemblyPath, m_currentDecomp.probingPaths);
}

void MainWindow::decompileType(TypeDefinitionHandle handle, const QString& assemblyPath, const QStringList& probingPaths)
{
    m_currentDecomp = {
        .assemblyPath = assemblyPath,
        .csVersion = CSVersionMap[ui->csVersionCombo->currentText()],
        .handle = handle,
        .language = DisplayLanguageMap[ui->languageCombo->currentText()],
        .probingPaths = probingPaths
    };

    if (QString code = Interface::decompileType(m_currentDecomp); !code.isEmpty())
    {
        if (m_currentDecomp.language == DisplayLanguage::CSharp)
        {
            if (decompILApp->settings().csImplicitUsings)
                StringUtils::applyImplicitUsings(code);
            if (decompILApp->settings().csStripILWarnings)
                StringUtils::stripILWarnings(code);
        }

        ui->codeEditor->setText(code, m_currentDecomp.language);
    }
}
