#include "assemblytreewidget.h"
#include "assemblyitemmenu.h"
#include "codeeditor/codeeditordefinitions.h"
#include "interface/interface.h"
#include "typetreeitem.h"
#include "ui/richtextitemdelegate.h"
#include <QDesktopServices>
#include <QDropEvent>
#include <QMessageBox>
#include <QMimeData>
#include <ranges>

AssemblyTreeWidget::AssemblyTreeWidget(QWidget* parent) : QTreeWidget(parent)
{
    setItemDelegate(new RichTextItemDelegate);
    viewport()->setAcceptDrops(true);

    connect(this, &AssemblyTreeWidget::customContextMenuRequested, this, &AssemblyTreeWidget::showItemMenu);
    connect(this, &AssemblyTreeWidget::itemDoubleClicked, this, &AssemblyTreeWidget::beginDecompile);
}

void AssemblyTreeWidget::closeAssembly(AssemblyTreeItem* item)
{
    m_loadedAssemblies.erase(item->path());
    takeTopLevelItem(indexOfTopLevelItem(item));
    delete item;
}

void AssemblyTreeWidget::closeAssemblies()
{
    clear();
    m_loadedAssemblies.clear();
}

void AssemblyTreeWidget::openAssembly(const QString& path)
{
    if (!m_loadedAssemblies.insert(path).second)
        return;

    QFileInfo fileInfo(path);
    if (auto assemblyInfo = Interface::getAssembly(path))
    {
        AssemblyTreeItem* assemblyItem = new AssemblyTreeItem(assemblyInfo->metadata, fileInfo);
        assemblyItem->addReferences(assemblyInfo->references);
        assemblyItem->addTypes(assemblyInfo->types);
        CodeEditorDefinitions::addTypes(assemblyInfo->types);
        addTopLevelItem(assemblyItem);
    }
    else
    {
        QMessageBox::critical(this, tr("Loading assembly failed"),
            fileInfo.fileName() + tr(" threw exception:\n") + assemblyInfo.error().what());
    }
}

void AssemblyTreeWidget::reloadAssembly(AssemblyTreeItem* item)
{
    m_loadedAssemblies.erase(item->path());
    openAssembly(item->path());
    delete item;
}

void AssemblyTreeWidget::openAssemblies(const QStringList& assemblies)
{
    for (const QString& assembly : assemblies)
    {
        openAssembly(assembly);
        QCoreApplication::processEvents();
    }
}

void AssemblyTreeWidget::reloadAssemblies()
{
    for (int i = 0; i < topLevelItemCount(); ++i)
    {
        if (AssemblyTreeItem* item = dynamic_cast<AssemblyTreeItem*>(takeTopLevelItem(0)))
            reloadAssembly(item);
        QCoreApplication::processEvents();
    }
}

// sortItems() exists, buuutt it's case sensitive :/ so we have to reimplement it
void AssemblyTreeWidget::sortAssemblies()
{
    QList<QTreeWidgetItem*> items(topLevelItemCount());
    for (int i = 0; i < items.size(); ++i)
        items[i] = topLevelItem(i);

    std::ranges::sort(items, [](const QTreeWidgetItem* lhs, const QTreeWidgetItem* rhs) {
        return lhs->text(0).compare(rhs->text(0), Qt::CaseInsensitive) < 0;
    });

    for (int i = 0; i < items.size(); ++i)
    {
        takeTopLevelItem(indexOfTopLevelItem(items[i]));
        insertTopLevelItem(i, items[i]);
    }
}

void AssemblyTreeWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls())
        setCursor(Qt::DragCopyCursor);
    QTreeWidget::dragEnterEvent(event);
}

void AssemblyTreeWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
    setCursor(QCursor());
    QTreeWidget::dragLeaveEvent(event);
}

void AssemblyTreeWidget::dropEvent(QDropEvent* event)
{
    setCursor(QCursor());
    if (event->mimeData()->hasUrls())
        openAssemblies(event->mimeData()->urls() | std::views::transform(&QUrl::toLocalFile) | std::ranges::to<QList>());
    QWidget::dropEvent(event); // want to defer to QWidget, otherwise this will add an entry to the tree
}

void AssemblyTreeWidget::beginDecompile(QTreeWidgetItem* item, int)
{
    if (const TypeTreeItem* typeItem = dynamic_cast<TypeTreeItem*>(item))
    {
        const AssemblyTreeItem* asmParentItem{};
        QTreeWidgetItem* parentItem = typeItem->parent();
        while (parentItem && !(asmParentItem = dynamic_cast<const AssemblyTreeItem*>(parentItem)))
            parentItem = parentItem->parent();

        if (!asmParentItem) [[unlikely]]
        {
            qCritical() << "Type item has no parent assembly item? How?";
            return;
        }

        emit decompileRequested(typeItem->handle(), asmParentItem->path(), asmParentItem->probingPaths());
    }
}

void AssemblyTreeWidget::openFolder(const QString& path)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void AssemblyTreeWidget::showItemMenu(const QPoint& pos)
{
    if (AssemblyTreeItem* item = dynamic_cast<AssemblyTreeItem*>(itemAt(pos)); item && item->childCount() > 0)
    {
        AssemblyItemMenu* menu = new AssemblyItemMenu(item, this);
        connect(menu, &AssemblyItemMenu::closeClicked, this, &AssemblyTreeWidget::closeAssembly);
        connect(menu, &AssemblyItemMenu::openFolderClicked, this, &AssemblyTreeWidget::openFolder);
        connect(menu, &AssemblyItemMenu::openReferencesClicked, this, &AssemblyTreeWidget::openAssemblies);
        connect(menu, &AssemblyItemMenu::reloadClicked, this, &AssemblyTreeWidget::reloadAssembly);
        menu->popup(mapToGlobal(pos));
    }
}
