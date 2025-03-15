#include "mainwindow.h"
#include "decompilapplication.h"
#include "ui_mainwindow.h"
#include "interface/interface.h"
#include "languagemapping.h"
#include "ui/richtextitemdelegate.h"
#include "ui/settingsform.h"
#include "ui/widgets/assemblytreeitem.h"
#include "ui/widgets/codeeditor/codeeditordefinitions.h"
#include "ui/widgets/findbar.h"
#include "ui/widgets/typetreeitem.h"
#include "utils/stringutils.h"
#include <QDesktopServices>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_codeEditor = ui->codeEditor;

    setWindowTitle(DECOMPIL_APP_NAME " v" DECOMPIL_VERSION_NAME);
    ui->treeWidget->setItemDelegate(new RichTextItemDelegate);

    connect(ui->closeAllAction, &QAction::triggered, this, &MainWindow::closeAssemblies);
    connect(ui->openAction, &QAction::triggered, this, &MainWindow::promptForAssemblies);
    connect(ui->reloadAllAction, &QAction::triggered, this, &MainWindow::reloadAssemblies);
    connect(ui->sortAssembliesAction, &QAction::triggered, this, &MainWindow::sortAssemblies);

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
    connect(ui->treeWidget, &AssemblyTreeWidget::filesDropped, this, &MainWindow::openAssemblies);
    connect(ui->treeWidget, &AssemblyTreeWidget::itemDoubleClicked, this, &MainWindow::treeItemDoubleClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openAssembly(const QString& path)
{
    QFileInfo fileInfo(path);
    if (auto assemblyInfo = Interface::getAssembly(path))
    {
        AssemblyTreeItem* assemblyItem = new AssemblyTreeItem(assemblyInfo->metadata, fileInfo);
        assemblyItem->addReferences(assemblyInfo->references);
        assemblyItem->addTypes(assemblyInfo->types);
        CodeEditorDefinitions::addTypes(assemblyInfo->types);
        ui->treeWidget->addTopLevelItem(assemblyItem);
    }
    else
    {
        QMessageBox::critical(this, tr("Loading assembly failed"),
            fileInfo.fileName() + tr(" threw exception:\n") + assemblyInfo.error().what());
    }
}

void MainWindow::openAssemblies(const QStringList& assemblies)
{
    for (const QString& assembly : assemblies)
    {
        openAssembly(assembly);
        QCoreApplication::processEvents();
    }
}

void MainWindow::closeAssemblies()
{
    ui->treeWidget->clear();
}

void MainWindow::promptForAssemblies()
{
    openAssemblies(QFileDialog::getOpenFileNames(
        this, tr("Choose .NET executable(s)"), QString(),
        tr(".NET Executables (*.exe *.dll *.netmodule *.winmd);;All Files (*)")));
}

void MainWindow::reloadAssemblies()
{
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
    {
        if (const AssemblyTreeItem* item = dynamic_cast<AssemblyTreeItem*>(ui->treeWidget->takeTopLevelItem(0)))
            openAssembly(item->path());
        QCoreApplication::processEvents();
    }
}

// sortItems() exists, buuutt it's case sensitive :/ so we have to reimplement it
void MainWindow::sortAssemblies()
{
    QList<QTreeWidgetItem*> items(ui->treeWidget->topLevelItemCount());
    for (int i = 0; i < items.size(); ++i)
        items[i] = ui->treeWidget->topLevelItem(i);

    std::ranges::sort(items, [](const QTreeWidgetItem* lhs, const QTreeWidgetItem* rhs) {
        return lhs->text(0).compare(rhs->text(0), Qt::CaseInsensitive) < 0;
    });

    for (int i = 0; i < items.size(); ++i)
    {
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(items[i]));
        ui->treeWidget->insertTopLevelItem(i, items[i]);
    }
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
    treeItemDoubleClicked(ui->treeWidget->currentItem(), 0);
}

void MainWindow::treeItemDoubleClicked(QTreeWidgetItem* item, int)
{
    if (const TypeTreeItem* typeItem = dynamic_cast<TypeTreeItem*>(item))
    {
        const AssemblyTreeItem* asmParentItem{};
        QTreeWidgetItem* parentItem = typeItem->parent();
        while (parentItem && !(asmParentItem = dynamic_cast<const AssemblyTreeItem*>(parentItem)))
            parentItem = parentItem->parent();

        if (!asmParentItem) [[unlikely]]
        {
            qCritical() << "Type item has no parent assembly item? How?";
            return;
        }

        DecompilationInfo decompInfo = {
            .assemblyPath = asmParentItem->path(),
            .csVersion = CSVersionMap[ui->csVersionCombo->currentText()],
            .language = DisplayLanguageMap[ui->languageCombo->currentText()],
            .referenceDirs = asmParentItem->probingPaths()
        };

        if (QString code = Interface::decompileType(typeItem->handle(), decompInfo); !code.isEmpty())
        {
            if (decompInfo.language == DisplayLanguage::CSharp)
            {
                if (decompILApp->settings().csImplicitUsings)
                    StringUtils::applyImplicitUsings(code);
                if (decompILApp->settings().csStripILWarnings)
                    StringUtils::stripILWarnings(code);
            }

            ui->codeEditor->setText(code, decompInfo.language);
        }
    }
}
