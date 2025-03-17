#include "assemblyitemmenu.h"
#include <QDir>

AssemblyItemMenu::AssemblyItemMenu(AssemblyTreeItem* item, QWidget* parent) : QMenu(parent)
{
    QAction* closeAction = new QAction(tr("Close"), this);
    connect(closeAction, &QAction::triggered, this,
        std::bind(&AssemblyItemMenu::closeClicked, this, item));
    addAction(closeAction);

    QAction* reloadAction = new QAction(tr("Reload"), this);
    connect(reloadAction, &QAction::triggered, this,
        std::bind(&AssemblyItemMenu::reloadClicked, this, item));
    addAction(reloadAction);

    QAction* openReferencesAction = new QAction(tr("Open References"), this);
    connect(openReferencesAction, &QAction::triggered, this,
        std::bind(&AssemblyItemMenu::openReferencesClicked, this, item->referencePaths()));
    addAction(openReferencesAction);

    QAction* openFolderAction = new QAction(tr("Open Containing Folder"), this);
    connect(openFolderAction, &QAction::triggered, this,
        std::bind(&AssemblyItemMenu::openFolderClicked, this, item->fileInfo().absoluteDir().path()));
    addAction(openFolderAction);
}
