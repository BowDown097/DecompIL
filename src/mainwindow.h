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
    void openAssembly(const QString& path);
private slots:
    void closeAssemblies();
    void openAssemblies();
    void reloadAssemblies();
    void sortAssemblies();

    void findInEditor();
    void redoInEditor();
    void searchAssemblies();
    void undoInEditor();

    void collapseTreeNodes();
    void openOptionsWindow();
    void toggleFullScreen();
    void toggleWordWrap();

    void goToRepo();
    void openAboutWindow();

    void handleItemDoubleClick(QTreeWidgetItem* item, int);
};
