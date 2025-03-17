#pragma once
#include <QMainWindow>
#include <set>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class AssemblyTreeItem;
class CodeEditor;
class FindBar;
class QTreeWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    static CodeEditor* codeEditor() { return m_codeEditor; }
private:
    inline static CodeEditor* m_codeEditor;
    std::set<QString> m_loadedAssemblies;

    Ui::MainWindow* ui;
private slots:
    void openAssembly(const QString& path);
    void openAssemblies(const QStringList& assemblies);

    void closeAssembly(AssemblyTreeItem* item);
    void closeAssemblies();

    void reloadAssembly(AssemblyTreeItem* item);
    void reloadAssemblies();

    void promptForAssemblies();
    void sortAssemblies();

    void findInEditor();
    void redoInEditor();
    void searchAssemblies();
    void undoInEditor();

    void collapseTreeNodes();
    void openSettingsForm();
    void toggleFullScreen();
    void toggleWordWrap();

    void goToRepo();
    void openAboutWindow();

    void comboBoxChanged(int);
    void treeItemDoubleClicked(QTreeWidgetItem* item, int);
    void treeItemRightClicked(const QPoint& pos);

    void openFolder(const QString& path);
};
