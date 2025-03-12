#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "interface.h"
#include "languagemapping.h"
#include "richtextitemdelegate.h"
#include "widgets/assemblytreeitem.h"
#include "widgets/codeeditor/codeeditordefinitions.h"
#include "widgets/findbar.h"
#include "widgets/typetreeitem.h"
#include <QDesktopServices>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    findBar = new FindBar(ui->codeEditor);

    setWindowTitle(DECOMPIL_APP_NAME " v" DECOMPIL_VERSION_NAME);
    ui->codeEditor->setTabStopDistance(ui->codeEditor->fontMetrics().horizontalAdvance(' ') * 4);
    ui->treeWidget->setItemDelegate(new RichTextItemDelegate);

    QAction* openAction = new QAction(QIcon(":/open_action.svg"), tr("Open"), this);
    QAction* reloadAllAction = new QAction(QIcon(":/reload_action.svg"), tr("Reload All Assemblies"), this);
    QAction* closeAllAction = new QAction(QIcon(":/close_all_action.svg"), tr("Close All"), this);
    QAction* sortAssembliesAction = new QAction(QIcon(":/sort_action.svg"), tr("Sort Assemblies"), this);

    QAction* undoAction = new QAction(QIcon(":/undo_action.svg"), tr("Undo"), this);
    QAction* redoAction = new QAction(QIcon(":/redo_action.svg"), tr("Redo"), this);
    QAction* findAction = new QAction(QIcon(":/find_action.svg"), tr("Find"), this);
    QAction* searchAssembliesAction = new QAction(QIcon(":/find_action.svg"), tr("Search Assemblies"), this);

    QAction* wordWrapAction = new QAction(QIcon(":/word_wrap_action.svg"), tr("Word Wrap"), this);
    QAction* fullScreenAction = new QAction(QIcon(":/fullscreen_action.svg"), tr("Full Screen"), this);
    QAction* collapseTreeNodesAction = new QAction(QIcon(":/collapse_action.svg"), tr("Collapse Tree Nodes"), this);
    QAction* optionsAction = new QAction(QIcon(":/options_action.svg"), tr("Options..."), this);

    QAction* sourceCodeAction = new QAction(tr("Source Code"), this);
    QAction* aboutAction = new QAction(tr("About"), this);

    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(reloadAllAction);
    fileMenu->addAction(closeAllAction);
    fileMenu->addAction(sortAssembliesAction);

    QMenu* editMenu = menuBar()->addMenu(tr("Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(findAction);
    editMenu->addAction(searchAssembliesAction);

    QMenu* viewMenu = menuBar()->addMenu(tr("View"));
    viewMenu->addAction(wordWrapAction);
    viewMenu->addAction(fullScreenAction);
    viewMenu->addAction(collapseTreeNodesAction);
    viewMenu->addSeparator();
    viewMenu->addAction(optionsAction);

    QMenu* helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(sourceCodeAction);
    helpMenu->addAction(aboutAction);

    connect(closeAllAction, &QAction::triggered, this, &MainWindow::closeAssemblies);
    connect(openAction, &QAction::triggered, this, &MainWindow::openAssemblies);
    connect(reloadAllAction, &QAction::triggered, this, &MainWindow::reloadAssemblies);
    connect(sortAssembliesAction, &QAction::triggered, this, &MainWindow::sortAssemblies);

    connect(findAction, &QAction::triggered, this, &MainWindow::findInEditor);
    connect(redoAction, &QAction::triggered, this, &MainWindow::redoInEditor);
    connect(searchAssembliesAction, &QAction::triggered, this, &MainWindow::searchAssemblies);
    connect(undoAction, &QAction::triggered, this, &MainWindow::undoInEditor);

    connect(collapseTreeNodesAction, &QAction::triggered, this, &MainWindow::collapseTreeNodes);
    connect(fullScreenAction, &QAction::triggered, this, &MainWindow::toggleFullScreen);
    connect(optionsAction, &QAction::triggered, this, &MainWindow::openOptionsWindow);
    connect(wordWrapAction, &QAction::triggered, this, &MainWindow::toggleWordWrap);

    connect(sourceCodeAction, &QAction::triggered, this, &MainWindow::goToRepo);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::openAboutWindow);

    connect(ui->csVersionCombo, &QComboBox::currentIndexChanged, this, &MainWindow::comboBoxChanged);
    connect(ui->languageCombo, &QComboBox::currentIndexChanged, this, &MainWindow::comboBoxChanged);
    connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, this, &MainWindow::treeItemDoubleClicked);
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

void MainWindow::closeAssemblies()
{
    ui->treeWidget->clear();
}

void MainWindow::openAssemblies()
{
    const QStringList assemblyPaths = QFileDialog::getOpenFileNames(
        this, tr("Choose .NET executable(s)"), QString(),
        tr(".NET Executables (*.exe *.dll *.netmodule *.winmd);;All Files (*)"));

    for (const QString& assemblyPath : assemblyPaths)
    {
        openAssembly(assemblyPath);
        QCoreApplication::processEvents();
    }
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
    findBar->setReveal(findBar->isHidden());
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

void MainWindow::openOptionsWindow()
{

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

        ui->codeEditor->setText(Interface::decompileType(typeItem->handle(), decompInfo), decompInfo.language);
    }
}
