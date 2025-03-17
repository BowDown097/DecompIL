#pragma once
#include "assemblytreeitem.h"
#include <QMenu>

class AssemblyItemMenu : public QMenu
{
    Q_OBJECT
public:
    explicit AssemblyItemMenu(AssemblyTreeItem* item, QWidget* parent = nullptr);
signals:
    void closeClicked(AssemblyTreeItem* item);
    void openFolderClicked(const QString& path);
    void openReferencesClicked(const QStringList& referencePaths);
    void reloadClicked(AssemblyTreeItem* item);
};
