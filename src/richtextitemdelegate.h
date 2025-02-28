#pragma once
#include <QStyledItemDelegate>

struct RichTextItemDelegate : QStyledItemDelegate
{
    explicit RichTextItemDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent) {}

    void paint(QPainter* painter, const QStyleOptionViewItem& inOption, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& inOption, const QModelIndex& index) const override;
};
