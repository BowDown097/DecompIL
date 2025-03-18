#pragma once
#include "interface/types/marshal/decompilationinfo.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class CodeEditor;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    static CodeEditor* codeEditor() { return m_codeEditor; }
private:
    inline static CodeEditor* m_codeEditor;

    DecompilationInfo m_currentDecomp{};
    Ui::MainWindow* ui;
private slots:
    void promptForAssemblies();

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
    void decompileType(TypeDefinitionHandle handle, const QString& assemblyPath, const QStringList& probingPaths);
};
