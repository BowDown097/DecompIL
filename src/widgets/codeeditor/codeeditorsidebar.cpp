#include "codeeditorsidebar.h"
#include "codeeditor.h"
#include <QTextBlock>

CodeEditorSidebar::CodeEditorSidebar(CodeEditor* editor)
    : QWidget(editor), m_codeEditor(editor) {}

QSize CodeEditorSidebar::sizeHint() const
{
    return QSize(m_codeEditor->sidebarWidth(), 0);
}

void CodeEditorSidebar::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->pos().x() >= width() - m_codeEditor->fontMetrics().lineSpacing())
    {
        QTextBlock block = m_codeEditor->blockAtPosition(event->pos().y());
        if (block.isValid() && m_codeEditor->isFoldable(block))
            m_codeEditor->toggleFold(block);
    }

    QWidget::mouseReleaseEvent(event);
}

void CodeEditorSidebar::paintEvent(QPaintEvent* event)
{
    m_codeEditor->sidebarPaintEvent(event);
}
