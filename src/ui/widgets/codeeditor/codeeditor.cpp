/*
    SPDX-FileCopyrightText: 2016 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#include "codeeditor.h"
#include "codeeditordefinitions.h"
#include "codeeditorsidebar.h"
#include "decompilapplication.h"
#include "ui/widgets/findbar.h"
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>
#include <QActionGroup>
#include <QMenu>
#include <QPainter>

CodeEditor::CodeEditor(QWidget* parent)
    : QPlainTextEdit(parent),
      m_findBar(new FindBar(this)),
      m_highlighter(new KSyntaxHighlighting::SyntaxHighlighter(document())),
      m_sidebar(new CodeEditorSidebar(this))
{
    QFont font;
    font.fromString(decompILApp->settings().editorFont);

    setFont(font);
    setTabWidth(decompILApp->settings().editorTabWidth);
    setTheme(CodeEditorDefinitions::repository().theme(decompILApp->settings().editorTheme));
    m_sidebar->setVisible(decompILApp->settings().editorShowSidebar);

    connect(this, &QPlainTextEdit::blockCountChanged, this, &CodeEditor::updateSidebarGeometry);
    connect(this, &QPlainTextEdit::updateRequest, this, &CodeEditor::updateSidebarArea);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    updateSidebarGeometry();
    highlightCurrentLine();
}

void CodeEditor::setTabWidth(int width)
{
    setTabStopDistance(fontMetrics().horizontalAdvance(' ') * width);
}

void CodeEditor::setText(const QString& text, DisplayLanguage language)
{
    clear();
    m_highlighter->setDefinition(language == DisplayLanguage::CSharp
        ? CodeEditorDefinitions::CSharpDefinition()
        : CodeEditorDefinitions::CILDefinition());
    setPlainText(text);
    m_findBar->updatePosition();
}

void CodeEditor::setTheme(const KSyntaxHighlighting::Theme& theme)
{
    if (theme.isValid())
    {
        QPalette pal = qApp->palette();
        pal.setColor(QPalette::Base, theme.editorColor(KSyntaxHighlighting::Theme::BackgroundColor));
        pal.setColor(QPalette::Highlight, theme.editorColor(KSyntaxHighlighting::Theme::TextSelection));
        setPalette(pal);
    }

    m_highlighter->setTheme(theme);
    m_highlighter->rehighlight();
    highlightCurrentLine();
}

void CodeEditor::updateSidebarGeometry()
{
    setViewportMargins(sidebarWidth(), 0, 0, 0);
    const QRect r = contentsRect();
    m_sidebar->setGeometry(r.left(), r.top(), sidebarWidth(), r.height());
}

void CodeEditor::contextMenuEvent(QContextMenuEvent* event)
{
    QScopedPointer<QMenu> menu(createStandardContextMenu(event->pos()));
    menu->addSeparator();

    // theme selection
    QActionGroup* themeGroup = new QActionGroup(menu.get());
    themeGroup->setExclusive(true);

    QMenu* themeMenu = menu->addMenu(QStringLiteral("Theme"));
    const QList<KSyntaxHighlighting::Theme> themes = CodeEditorDefinitions::repository().themes();

    for (const KSyntaxHighlighting::Theme& theme : themes)
    {
        QAction* action = themeMenu->addAction(theme.translatedName());
        action->setCheckable(true);
        action->setData(theme.name());
        themeGroup->addAction(action);

        if (theme.name() == m_highlighter->theme().name())
            action->setChecked(true);
    }

    connect(themeGroup, &QActionGroup::triggered, this, [this](QAction* action) {
        setTheme(CodeEditorDefinitions::repository().theme(action->data().toString()));
    });

    menu->exec(event->globalPos());
}

void CodeEditor::resizeEvent(QResizeEvent* event)
{
    QPlainTextEdit::resizeEvent(event);
    updateSidebarGeometry();
}

void CodeEditor::highlightCurrentLine()
{
    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(QColor(m_highlighter->theme().editorColor(KSyntaxHighlighting::Theme::CurrentLine)));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();

    QList<QTextEdit::ExtraSelection> extraSelections;
    extraSelections.append(selection);
    setExtraSelections(extraSelections);
}

void CodeEditor::sidebarPaintEvent(QPaintEvent* event)
{
    QPainter painter(m_sidebar);
    painter.fillRect(event->rect(), m_highlighter->theme().editorColor(KSyntaxHighlighting::Theme::IconBorder));

    QTextBlock block = firstVisibleBlock();
    int top = blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + blockBoundingRect(block).height();
    const int currentBlockNumber = textCursor().blockNumber();
    const int foldingMarkerSize = fontMetrics().lineSpacing();

    for (int blockNumber = block.blockNumber(); block.isValid() && top <= event->rect().bottom(); ++blockNumber)
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            const QString number = QString::number(blockNumber + 1);
            painter.setPen(m_highlighter->theme().editorColor(blockNumber == currentBlockNumber
                ? KSyntaxHighlighting::Theme::CurrentLineNumber : KSyntaxHighlighting::Theme::LineNumbers));
            painter.drawText(0, top, m_sidebar->width() - 2 - foldingMarkerSize, fontMetrics().height(), Qt::AlignRight, number);
        }

        // folding marker
        if (block.isVisible() && isFoldable(block))
        {
            QPolygonF polygon;

            if (isFolded(block))
            {
                polygon << QPointF(foldingMarkerSize * 0.4, foldingMarkerSize * 0.25);
                polygon << QPointF(foldingMarkerSize * 0.4, foldingMarkerSize * 0.75);
                polygon << QPointF(foldingMarkerSize * 0.8, foldingMarkerSize * 0.5);
            }
            else
            {
                polygon << QPointF(foldingMarkerSize * 0.25, foldingMarkerSize * 0.4);
                polygon << QPointF(foldingMarkerSize * 0.75, foldingMarkerSize * 0.4);
                polygon << QPointF(foldingMarkerSize * 0.5, foldingMarkerSize * 0.8);
            }

            painter.save();
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(m_highlighter->theme().editorColor(KSyntaxHighlighting::Theme::CodeFolding)));
            painter.translate(m_sidebar->width() - foldingMarkerSize, top);
            painter.drawPolygon(polygon);
            painter.restore();
        }

        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
    }
}

int CodeEditor::sidebarWidth() const
{
    if (m_sidebar->isHidden())
        return 0;
    int digits = 1;
    for (int count = blockCount(); count >= 10; ++digits, count /= 10);
    return 4 + fontMetrics().horizontalAdvance('9') * digits + fontMetrics().lineSpacing();
}

void CodeEditor::updateSidebarArea(const QRect& rect, int dy)
{
    if (dy)
        m_sidebar->scroll(0, dy);
    else
        m_sidebar->update(0, rect.y(), m_sidebar->width(), rect.height());
}

QTextBlock CodeEditor::blockAtPosition(int y) const
{
    if (QTextBlock block = firstVisibleBlock(); block.isValid())
    {
        int top = blockBoundingGeometry(block).translated(contentOffset()).top();
        int bottom = top + blockBoundingRect(block).height();

        do
        {
            if (top <= y && y <= bottom)
                return block;

            block = block.next();
            top = bottom;
            bottom = top + blockBoundingRect(block).height();
        } while (block.isValid());
    }

    return QTextBlock();
}

bool CodeEditor::isFoldable(const QTextBlock& block) const
{
    return m_highlighter->startsFoldingRegion(block);
}

bool CodeEditor::isFolded(const QTextBlock& block) const
{
    if (!block.isValid())
        return false;

    const QTextBlock nextBlock = block.next();
    if (!nextBlock.isValid())
        return false;

    return !nextBlock.isVisible();
}

void CodeEditor::toggleFold(const QTextBlock& startBlock)
{
    // we also want to fold the last line of the region, therefore the ".next()"
    const QTextBlock endBlock = m_highlighter->findFoldingRegionEnd(startBlock).next();

    if (isFolded(startBlock))
    {
        // unfold
        for (QTextBlock block = startBlock.next(); block.isValid() && !block.isVisible(); block = block.next())
        {
            block.setVisible(true);
            block.setLineCount(block.layout()->lineCount());
        }
    }
    else
    {
        // fold
        for (QTextBlock block = startBlock.next(); block.isValid() && block != endBlock; block = block.next())
        {
            block.setVisible(false);
            block.setLineCount(0);
        }
    }

    // redraw document
    document()->markContentsDirty(startBlock.position(), endBlock.position() - startBlock.position() + 1);

    // update scrollbars
    emit document()->documentLayout()->documentSizeChanged(document()->documentLayout()->documentSize());
}
