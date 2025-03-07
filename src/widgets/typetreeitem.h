#pragma once
#include "interface_types/native/assemblytypemetadata.h"
#include <QTreeWidgetItem>

class TypeTreeItem : public QTreeWidgetItem
{
public:
    TypeTreeItem(const NativeTypes::AssemblyTypeMetadata& metadata, QTreeWidget* treeview = nullptr);
    TypeDefinitionHandle handle() const { return m_handle; }
private:
    TypeDefinitionHandle m_handle;
};
