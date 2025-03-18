#pragma once
#include "interface/types/marshal/assemblytypes/shared.h"
#include <QTreeWidget>
#include <set>

class AssemblyTreeItem;

class AssemblyTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit AssemblyTreeWidget(QWidget* parent = nullptr);

    void closeAssembly(AssemblyTreeItem* item);
    void openAssembly(const QString& path);
    void reloadAssembly(AssemblyTreeItem* item);

    void closeAssemblies();
    void openAssemblies(const QStringList& assemblies);
    void reloadAssemblies();
    void sortAssemblies();
protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent*) override;
    void dropEvent(QDropEvent* event) override;
private:
    std::set<QString> m_loadedAssemblies;
private slots:
    void beginDecompile(QTreeWidgetItem* item, int);
    void openFolder(const QString& path);
    void showItemMenu(const QPoint& pos);
signals:
    void decompileRequested(TypeDefinitionHandle handle, const QString& assemblyPath, const QStringList& probingPaths);
};
