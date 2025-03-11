#pragma once
#include <QStyledItemDelegate>

struct RichTextItemDelegate : QStyledItemDelegate
{
    using QStyledItemDelegate::QStyledItemDelegate;
    void paint(QPainter* painter, const QStyleOptionViewItem& inOption, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& inOption, const QModelIndex& index) const override;
};
