#pragma once
#include "interface_types/native/assemblymetadata.h"
#include "interface_types/native/assemblytypemetadata.h"
#include <QTreeWidgetItem>

class AssemblyTreeItem : public QTreeWidgetItem
{
public:
    AssemblyTreeItem(const NativeTypes::AssemblyMetadata& metadata, const QString& path, QTreeWidget* treeview = nullptr);
    void addReferences(const QList<NativeTypes::AssemblyMetadata>& references, const QString& asmPath);
    void addTypes(QList<NativeTypes::AssemblyTypeMetadata>& types); // plain reference because types will be sorted
private:
    static QString locateReferenceAssembly(const QString& asmPath, const QString& refName);
};
