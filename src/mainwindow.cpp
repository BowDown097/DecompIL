#include "mainwindow.h"
#include "interface.h"
#include "richtextitemdelegate.h"
#include "ui_mainwindow.h"
#include "widgets/assemblytreeitem.h"
#include "widgets/codeeditor/codeeditordefinitions.h"
#include "widgets/typetreeitem.h"
#include <QDesktopServices>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    connect(fullScreenAction, &QAction::triggered, this, &MainWindow::toggleFullScreen);
    connect(openAction, &QAction::triggered, this, &MainWindow::openAssemblies);
    connect(sourceCodeAction, &QAction::triggered, this, &MainWindow::goToRepo);
    connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, this, &MainWindow::handleItemDoubleClick);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::goToRepo()
{
    QDesktopServices::openUrl(QUrl(DECOMPIL_REPO_URL));
}

void MainWindow::handleItemDoubleClick(QTreeWidgetItem* item, int)
{
    if (TypeTreeItem* typeItem = dynamic_cast<TypeTreeItem*>(item))
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

        ui->codeEditor->setText(
            Interface::decompileType(asmParentItem->path(), typeItem->handle(), asmParentItem->probingPaths()),
            CodeEditor::DisplayLanguage::CSharp);
    }
}

void MainWindow::openAssemblies()
{
    const QStringList assemblyPaths = QFileDialog::getOpenFileNames(
        this, tr("Choose .NET executable(s)"), QString(),
        tr(".NET Executables (*.exe *.dll *.netmodule *.winmd);;All Files (*)"));

    for (const QString& assemblyPath : assemblyPaths)
    {
        QFileInfo assemblyFile(assemblyPath);

        if (auto assemblyInfo = Interface::getAssembly(assemblyPath))
        {
            AssemblyTreeItem* assemblyItem = new AssemblyTreeItem(assemblyInfo->metadata, assemblyFile);
            assemblyItem->addReferences(assemblyInfo->references);
            assemblyItem->addTypes(assemblyInfo->types);
            CodeEditorDefinitions::addTypes(assemblyInfo->types);
            ui->treeWidget->addTopLevelItem(assemblyItem);
        }
        else
        {
            QMessageBox::critical(this, tr("Loading assembly failed"),
                assemblyFile.fileName() + tr(" threw exception:\n") + assemblyInfo.error().what());
        }

        QCoreApplication::processEvents();
    }
}

void MainWindow::toggleFullScreen()
{
    if (windowState() == Qt::WindowFullScreen)
        showNormal();
    else
        showFullScreen();
}
