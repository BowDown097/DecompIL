#include "assemblytreewidget.h"
#include <QDropEvent>
#include <QMimeData>
#include <ranges>

AssemblyTreeWidget::AssemblyTreeWidget(QWidget* parent) : QTreeWidget(parent)
{
    viewport()->setAcceptDrops(true);
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
        emit filesDropped(event->mimeData()->urls() | std::views::transform(&QUrl::toLocalFile) | std::ranges::to<QList>());
    QWidget::dropEvent(event); // want to defer to QWidget, otherwise this will add an entry to the tree
}
