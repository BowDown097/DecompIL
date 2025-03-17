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

    const QFileInfo& fileInfo() const { return m_fileInfo; }
    const QString& path() const { return m_path; }
    const QStringList& probingPaths() const { return m_probingPaths; }
    const QStringList& referencePaths() const { return m_referencePaths; }
private:
    QFileInfo m_fileInfo;
    QString m_path;
    QStringList m_probingPaths;
    QStringList m_referencePaths;

    void grabProbingPaths(const QString& asmDir);
};
