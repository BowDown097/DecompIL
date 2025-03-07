#pragma once
#include <QWidget>

class CodeEditor;

class CodeEditorSidebar : public QWidget
{
    Q_OBJECT
public:
    explicit CodeEditorSidebar(CodeEditor* editor);
    QSize sizeHint() const override;
protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
private:
    CodeEditor* m_codeEditor;
};
