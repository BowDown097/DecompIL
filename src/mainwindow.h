#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class CodeEditor;
class QTreeWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow* ui;
private slots:
    void goToRepo();
    void handleItemDoubleClick(QTreeWidgetItem* item, int);
    void openExecutables();
    void toggleFullScreen();
};
