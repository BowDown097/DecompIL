#pragma once
#include "interface_types/native/assemblytypemetadata.h"
#include <QTreeWidgetItem>

class TypeTreeItem : public QTreeWidgetItem
{
public:
    TypeTreeItem(const NativeTypes::AssemblyTypeMetadata& metadata, QTreeWidget* treeview = nullptr);
    int handle() const { return m_handle; }
private:
    int m_handle;
};
