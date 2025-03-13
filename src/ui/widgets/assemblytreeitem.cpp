#include "assemblytreeitem.h"
#include "typetreeitem.h"
#include "utils/uiutils.h"
#include <QDirIterator>
#include <QXmlStreamReader>
#include <ranges>

#ifndef __cpp_lib_ranges_concat
#include "utils/views_concat/concat.hpp"
#endif

namespace detail
{
    QFileInfo locateReferenceAssembly(std::ranges::view auto searchDirs, const QString& refName)
    {
        static constexpr std::array DotNetExtensions = { ".dll", ".exe", ".netmodule", ".winmd" };

        for (const QString& searchDir : searchDirs)
        {
            QDirIterator it(searchDir, QDir::Files);
            while (it.hasNext())
            {
                if (QFileInfo fileInfo(it.next());
                    std::ranges::any_of(DotNetExtensions, [fileName = fileInfo.fileName(), &refName](const char* ext) {
                        return fileName == refName + ext;
                    }))
                {
                    return fileInfo;
                }
            }
        }

        return {};
    }
}

AssemblyTreeItem::AssemblyTreeItem(const NativeTypes::AssemblyMetadata& metadata,
                                   const QFileInfo& fileInfo,
                                   QTreeWidget* treeview)
    : QTreeWidgetItem(treeview), m_fileInfo(fileInfo), m_path(fileInfo.absoluteFilePath())
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
    if (qsizetype nameind = m_path.lastIndexOf(metadata.name); nameind != -1)
        tooltip += QString(m_path).replace(nameind, metadata.name.size(), UIUtils::formattedValue(metadata.name, "#4B8595"));
    tooltip += "</p>";

    setToolTip(0, tooltip);
}

void AssemblyTreeItem::addReferences(const QList<NativeTypes::AssemblyMetadata>& references)
{
    if (references.empty())
        return;

    QString asmDir = m_fileInfo.dir().absolutePath();

    if (m_probingPaths.empty())
        grabProbingPaths(asmDir);

    QTreeWidgetItem* referencesItem = new QTreeWidgetItem;
    referencesItem->setText(0, QObject::tr("References"));

    auto searchDirs = std::views::concat(std::views::single(asmDir), m_probingPaths);

    for (const NativeTypes::AssemblyMetadata& refasm : references)
    {
        QFileInfo refInfo = detail::locateReferenceAssembly(searchDirs, refasm.name);
        if (!refInfo.exists())
            qDebug() << QObject::tr("Failed to locate path for reference assembly") << refasm.name;
        referencesItem->addChild(new AssemblyTreeItem(refasm, refInfo));
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

void AssemblyTreeItem::grabProbingPaths(const QString& asmDir)
{
    QFile configFile(m_path + ".config");

    if (configFile.open(QFile::ReadOnly))
    {
        QXmlStreamReader xmlReader(&configFile);
        while (!xmlReader.atEnd())
        {
            if (xmlReader.readNext() == QXmlStreamReader::StartElement && xmlReader.name() == "probing")
            {
                const QStringList privatePaths = QString(xmlReader.attributes().value("privatePath")).split(';');
                for (const QString& privatePath : privatePaths)
                {
                    if (!QDir::isAbsolutePath(privatePath))
                        m_probingPaths.append(QDir::cleanPath(asmDir + '/' + privatePath));
                    else
                        m_probingPaths.append(privatePath);
                }
            }
        }
    }
}
