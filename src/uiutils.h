#pragma once
#include "interface_types/native/assemblymetadata.h"
#include "interface_types/native/assemblytypemetadata.h"
#include <QTreeWidgetItem>

namespace UIUtils
{
    QTreeWidgetItem* createAssemblyItem(const NativeTypes::AssemblyMetadata& metadata, const QString& path);
    QTreeWidgetItem* createTypeItem(const NativeTypes::AssemblyTypeMetadata& metadata);
    QString formattedValue(const QString& value, const QString& color);
    QString formattedKeyValue(const QString& key, const QString& keyColor, const QString& value, const QString& valueColor);
}
