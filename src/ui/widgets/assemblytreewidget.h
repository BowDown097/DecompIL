#pragma once
#include <QTreeWidget>

class AssemblyTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit AssemblyTreeWidget(QWidget* parent = nullptr);
protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent*) override;
    void dropEvent(QDropEvent* event) override;
signals:
    void filesDropped(const QStringList& files);
};
