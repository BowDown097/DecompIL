#pragma once
#include "interface/types/native/assemblymetadata.h"
#include "interface/types/native/assemblytypemetadata.h"
#include <QFileInfo>
#include <QTreeWidgetItem>

class AssemblyTreeItem : public QTreeWidgetItem
{
public:
    AssemblyTreeItem(const NativeTypes::AssemblyMetadata& metadata,
                     const QFileInfo& fileInfo,
                     QTreeWidget* treeview = nullptr);

    void addReferences(const QList<NativeTypes::AssemblyMetadata>& references);
    void addTypes(QList<NativeTypes::AssemblyTypeMetadata>& types); // plain reference because types will be sorted

    const QString& path() const { return m_path; }
    const QStringList& probingPaths() const { return m_probingPaths; }
private:
    QFileInfo m_fileInfo;
    QString m_path;
    QStringList m_probingPaths;

    void grabProbingPaths(const QString& asmDir);
};
