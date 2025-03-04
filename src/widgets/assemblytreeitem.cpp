#include "assemblytreeitem.h"
#include "typetreeitem.h"
#include "uiutils.h"
#include <QDirIterator>
#include <ranges>

AssemblyTreeItem::AssemblyTreeItem(const NativeTypes::AssemblyMetadata& metadata, const QString& path, QTreeWidget* treeview)
    : QTreeWidgetItem(treeview)
{
    setText(0, metadata.name + " (" + UIUtils::formattedValue(metadata.version, "#B5CEA8") + ')');

    QString tooltip = "<style>p { margin: 0 0 0 0 }</style><p style='white-space:pre'>";
    if (!metadata.name.isEmpty())
        tooltip += metadata.name + ", ";
    if (!metadata.version.isEmpty())
        tooltip += UIUtils::formattedKeyValue("Version", "#248F8F", metadata.version, "#B5CEA8") + ", ";
    if (!metadata.culture.isEmpty())
        tooltip += UIUtils::formattedKeyValue("Culture", "#248F8F", metadata.culture, "#BD63C5") + ", ";
    if (!metadata.publicKeyToken.isEmpty())
        tooltip += UIUtils::formattedKeyValue("PublicKeyToken", "#248F8F", metadata.publicKeyToken, "#B5CEA8") + '\n';
    if (!metadata.commonAttributes.targetFramework.isEmpty())
        tooltip += metadata.commonAttributes.targetFramework + '\n';
    if (qsizetype nameind = path.lastIndexOf(metadata.name); nameind != -1)
        tooltip += QString(path).replace(nameind, metadata.name.size(), UIUtils::formattedValue(metadata.name, "#4B8595"));
    tooltip += "</p>";

    setToolTip(0, tooltip);
}

void AssemblyTreeItem::addReferences(const QList<NativeTypes::AssemblyMetadata>& references, const QString& asmPath)
{
    if (references.empty())
        return;

    QTreeWidgetItem* referencesItem = new QTreeWidgetItem;
    referencesItem->setText(0, QObject::tr("References"));

    for (const NativeTypes::AssemblyMetadata& refasm : references)
    {
        QString refPath = locateReferenceAssembly(asmPath, refasm.name);
        if (refPath.isEmpty())
            qDebug() << QObject::tr("Failed to locate path for reference assembly") << refasm.name;
        referencesItem->addChild(new AssemblyTreeItem(refasm, refPath));
    }

    addChild(referencesItem);
}

void AssemblyTreeItem::addTypes(QList<NativeTypes::AssemblyTypeMetadata>& types)
{
    std::ranges::sort(types, [](const auto& lhs, const auto& rhs) {
        int comparison = lhs.namespaceName.compare(rhs.namespaceName, Qt::CaseInsensitive);
        if (comparison < 0) return true;
        if (comparison > 0) return false;
        return lhs.name.compare(rhs.name, Qt::CaseInsensitive) < 0;
    });

    auto chunkedTypes = std::views::chunk_by(types, [](const auto& lhs, const auto& rhs) {
        return lhs.namespaceName == rhs.namespaceName;
    });

    for (const auto& subrange : chunkedTypes)
    {
        const NativeTypes::AssemblyTypeMetadata& first = *subrange.begin();

        QTreeWidgetItem* namespaceItem = new QTreeWidgetItem;
        if (!first.namespaceName.isEmpty())
            namespaceItem->setText(0, UIUtils::formattedValue(first.namespaceName, "#FFD700"));
        else
            namespaceItem->setText(0, "-");

        for (const NativeTypes::AssemblyTypeMetadata& type : subrange)
            if (!type.isCompilerGenerated)
                namespaceItem->addChild(new TypeTreeItem(type));

        addChild(namespaceItem);
    }
}

QString AssemblyTreeItem::locateReferenceAssembly(const QString& asmPath, const QString& refName)
{
    QDirIterator it(asmPath, QDir::Files, QDirIterator::Subdirectories);
    static constexpr std::array DotNetExtensions = { ".exe", ".dll", ".netmodule", ".winmd" };

    while (it.hasNext())
    {
        if (QFileInfo fileInfo(it.next());
            std::ranges::any_of(DotNetExtensions, [fileName = fileInfo.fileName(), &refName](const char* ext) {
                return fileName == refName + ext;
            }))
        {
            return fileInfo.absoluteFilePath();
        }
    }

    return QString();
}
