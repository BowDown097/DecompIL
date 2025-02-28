#include "interface.h"
#include "mainwindow.h"
#include "richtextitemdelegate.h"
#include "ui_mainwindow.h"
#include "uiutils.h"
#include <QDesktopServices>
#include <QDirIterator>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <ranges>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(DECOMPIL_APP_NAME " v" DECOMPIL_VERSION_NAME);
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
    connect(openAction, &QAction::triggered, this, &MainWindow::openExecutables);
    connect(sourceCodeAction, &QAction::triggered, this, &MainWindow::goToRepo);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::goToRepo()
{
    QDesktopServices::openUrl(QUrl(DECOMPIL_REPO_URL));
}

void MainWindow::openExecutables()
{
    const QStringList executables = QFileDialog::getOpenFileNames(
        this, tr("Choose .NET executable(s)"), QString(),
        tr(".NET Executables (*.exe *.dll *.netmodule *.winmd);;All Files (*)"));

    for (const QString& executable : executables)
    {
        QFileInfo executableInfo(executable);

        if (auto assemblyInfo = Interface::getAssembly(executable))
        {
            QTreeWidgetItem* item = UIUtils::createAssemblyItem(assemblyInfo->metadata, executable);

            if (!assemblyInfo->references.empty())
            {
                QTreeWidgetItem* referencesItem = new QTreeWidgetItem;
                referencesItem->setText(0, tr("References"));

                for (const NativeTypes::AssemblyMetadata& refasm : std::as_const(assemblyInfo->references))
                {
                    QDirIterator it(executableInfo.dir().absolutePath(), QDir::Files, QDirIterator::Subdirectories);

                    QString path;
                    while (it.hasNext())
                    {
                        if (QFileInfo fileInfo(it.next()); fileInfo.fileName() == refasm.name + ".dll")
                        {
                            path = fileInfo.absoluteFilePath();
                            break;
                        }
                    }

                    referencesItem->addChild(UIUtils::createAssemblyItem(refasm, path));
                }

                item->addChild(referencesItem);
            }

            std::ranges::sort(assemblyInfo->types, [](const auto& lhs, const auto& rhs) {
                int comparison = lhs.namespaceName.compare(rhs.namespaceName, Qt::CaseInsensitive);
                if (comparison < 0) return true;
                if (comparison > 0) return false;
                return lhs.name.compare(rhs.name, Qt::CaseInsensitive) < 0;
            });

            auto chunkedTypes = std::views::chunk_by(assemblyInfo->types, [](const auto& lhs, const auto& rhs) {
                return lhs.namespaceName == rhs.namespaceName;
            });

            for (const auto& subrange : chunkedTypes)
            {
                const NativeTypes::AssemblyTypeMetadata& first = *subrange.begin();

                QTreeWidgetItem* namespaceItem = new QTreeWidgetItem;
                if (!first.namespaceName.isEmpty())
                    namespaceItem->setText(0, UIUtils::formattedValue(first.namespaceName, "#FFD700"));
                else
                    namespaceItem->setText(0, "-");

                for (const NativeTypes::AssemblyTypeMetadata& type : subrange)
                    namespaceItem->addChild(UIUtils::createTypeItem(type));

                item->addChild(namespaceItem);
            }

            ui->treeWidget->addTopLevelItem(item);
        }
        else
        {
            QMessageBox::critical(this, tr("Loading assembly failed"),
                executableInfo.fileName() + tr(" threw exception:\n") + assemblyInfo.error().what());
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
