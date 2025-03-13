#include "typetreeitem.h"
#include "utils/uiutils.h"

TypeTreeItem::TypeTreeItem(const NativeTypes::AssemblyTypeMetadata& metadata, QTreeWidget* treeview)
    : QTreeWidgetItem(treeview), m_handle(metadata.handle)
{
    setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

    QString tooltip = "<style>p { margin: 0 0 0 0 }</style><p style='white-space:pre'>";

    if (metadata.isReadOnly)
        tooltip += UIUtils::formattedValue("readonly", "#4B8595") + ' ';

    QString typeColor;

    switch (metadata.kind)
    {
    case TypeKind::Class:
        tooltip += UIUtils::formattedValue("class", "#4B8595") + ' ';
        if (metadata.isStatic())
            typeColor = "#378D7B";
        else
            typeColor = "#4EC9B0";
        break;
    case TypeKind::Delegate:
        tooltip += UIUtils::formattedValue("delegate", "#4B8595") + ' ';
        typeColor = "#33CCFF";
        break;
    case TypeKind::Enum:
        tooltip += UIUtils::formattedValue("enum", "#4B8595") + ' ';
        typeColor = "#B8D7A3";
        break;
    case TypeKind::Interface:
        tooltip += UIUtils::formattedValue("interface", "#4B8595") + ' ';
        typeColor = "#9B9B82";
        break;
    case TypeKind::Struct:
        tooltip += UIUtils::formattedValue("struct", "#4B8595") + ' ';
        typeColor = "#009933";
        break;
    }

    if (!metadata.namespaceName.isEmpty())
        tooltip += UIUtils::formattedValue(metadata.namespaceName, "#FFD700") + '.';

    QString formattedName = UIUtils::formattedValue(metadata.name.toHtmlEscaped(), typeColor);
    tooltip += formattedName;

    tooltip += "</p>";

    setText(0, formattedName);
    setToolTip(0, tooltip);

    for (const NativeTypes::AssemblyTypeMetadata& nestedType : metadata.nestedTypes)
        if (!nestedType.isCompilerGenerated)
            addChild(new TypeTreeItem(nestedType, treeview));
}
