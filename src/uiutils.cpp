#include "uiutils.h"

namespace UIUtils
{
    QString formattedValue(const QString& value, const QString& color)
    {
        return QStringLiteral("<font color='%1'>%2</font>").arg(color, value);
    }

    QString formattedKeyValue(const QString& key, const QString& keyColor, const QString& value, const QString& valueColor)
    {
        return formattedValue(key, keyColor) + '=' + formattedValue(value, valueColor);
    }

    QTreeWidgetItem* createAssemblyItem(const NativeTypes::AssemblyMetadata& metadata, const QString& path)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0, metadata.name + " (" + formattedValue(metadata.version, "#B5CEA8") + ')');

        QString tooltip = "<style>p { margin: 0 0 0 0 }</style><p style='white-space:pre'>";
        if (!metadata.name.isEmpty())
            tooltip += metadata.name + ", ";
        if (!metadata.version.isEmpty())
            tooltip += formattedKeyValue("Version", "#248F8F", metadata.version, "#B5CEA8") + ", ";
        if (!metadata.culture.isEmpty())
            tooltip += formattedKeyValue("Culture", "#248F8F", metadata.culture, "#BD63C5") + ", ";
        if (!metadata.publicKeyToken.isEmpty())
            tooltip += formattedKeyValue("PublicKeyToken", "#248F8F", metadata.publicKeyToken, "#B5CEA8") + '\n';
        if (!metadata.commonAttributes.targetFramework.isEmpty())
            tooltip += metadata.commonAttributes.targetFramework + '\n';
        if (qsizetype nameind = path.lastIndexOf(metadata.name); nameind != -1)
            tooltip += QString(path).replace(nameind, metadata.name.size(), formattedValue(metadata.name, "#4B8595"));
        tooltip += "</p>";

        item->setToolTip(0, tooltip);
        return item;
    }

    QTreeWidgetItem* createTypeItem(const NativeTypes::AssemblyTypeMetadata& metadata)
    {
        if (metadata.isCompilerGenerated)
            return nullptr;

        QTreeWidgetItem* item = new QTreeWidgetItem;

        QString tooltip = "<style>p { margin: 0 0 0 0 }</style><p style='white-space:pre'>";

        if (metadata.isReadOnly)
            tooltip += formattedValue("readonly", "#4B8595") + ' ';

        QString escapedName = metadata.name.toHtmlEscaped();
        QString typeColor;

        switch (metadata.kind)
        {
        case TypeKind::Class:
            tooltip += formattedValue("class", "#4B8595") + ' ';
            if (metadata.isStatic())
                typeColor = "#378D7B";
            else
                typeColor = "#4EC9B0";
            break;
        case TypeKind::Delegate:
            tooltip += formattedValue("delegate", "#4B8595") + ' ';
            typeColor = "#33CCFF";
            break;
        case TypeKind::Enum:
            tooltip += formattedValue("enum", "#4B8595") + ' ';
            typeColor = "#B8D7A3";
            break;
        case TypeKind::Interface:
            tooltip += formattedValue("interface", "#4B8595") + ' ';
            typeColor = "#9B9B82";
            break;
        case TypeKind::Struct:
            tooltip += formattedValue("struct", "#4B8595") + ' ';
            typeColor = "#009933";
            break;
        }

        if (!metadata.namespaceName.isEmpty())
            tooltip += formattedValue(metadata.namespaceName, "#FFD700") + '.';

        tooltip += formattedValue(escapedName, typeColor);

        tooltip += "</p>";

        item->setText(0, formattedValue(escapedName, typeColor));
        item->setToolTip(0, tooltip);

        for (const NativeTypes::AssemblyTypeMetadata& nestedType : metadata.nestedTypes)
            item->addChild(createTypeItem(nestedType));

        return item;
    }
}
