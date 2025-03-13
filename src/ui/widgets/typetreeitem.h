#pragma once
#include "interface/types/native/assemblytypemetadata.h"
#include <QTreeWidgetItem>

class TypeTreeItem : public QTreeWidgetItem
{
public:
    explicit TypeTreeItem(const NativeTypes::AssemblyTypeMetadata& metadata, QTreeWidget* treeview = nullptr);
    TypeDefinitionHandle handle() const { return m_handle; }
private:
    TypeDefinitionHandle m_handle;
};
