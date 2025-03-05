/*
    SPDX-FileCopyrightText: 2016 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#include "codeeditor.h"
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/FoldingRegion>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>
#include <QActionGroup>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFontDatabase>
#include <QMenu>
#include <QPainter>
#include <QPalette>

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

CodeEditor::CodeEditor(QWidget* parent)
    : QPlainTextEdit(parent),
      m_highlighter(new KSyntaxHighlighting::SyntaxHighlighter(document())),
      m_sideBar(new CodeEditorSidebar(this))
{
    std::call_once(m_onceFlag, [] { m_repository = new KSyntaxHighlighting::Repository; });

    setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    setTheme(palette().color(QPalette::Base).lightness() < 128
        ? m_repository->defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
        : m_repository->defaultTheme(KSyntaxHighlighting::Repository::LightTheme));

    connect(this, &QPlainTextEdit::blockCountChanged, this, &CodeEditor::updateSidebarGeometry);
    connect(this, &QPlainTextEdit::updateRequest, this, &CodeEditor::updateSidebarArea);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    updateSidebarGeometry();
    highlightCurrentLine();
}

void CodeEditor::setText(const QString& text, const QString& defName)
{
    clear();
    m_highlighter->setDefinition(m_repository->definitionForName(defName));
    setPlainText(text);
}

void CodeEditor::contextMenuEvent(QContextMenuEvent* event)
{
    QScopedPointer<QMenu> menu(createStandardContextMenu(event->pos()));
    menu->addSeparator();

    // syntax selection
    QActionGroup* hlActionGroup = new QActionGroup(menu.get());
    hlActionGroup->setExclusive(true);

    QMenu* hlGroupMenu = menu->addMenu(QStringLiteral("Syntax"));
    QMenu* hlSubMenu = hlGroupMenu;

    QString currentGroup;
    const QList<KSyntaxHighlighting::Definition> definitions = m_repository->definitions();

    for (const KSyntaxHighlighting::Definition& def : definitions)
    {
        if (def.isHidden())
            continue;

        if (currentGroup != def.section())
        {
            currentGroup = def.section();
            hlSubMenu = hlGroupMenu->addMenu(def.translatedSection());
        }

        QAction* action = hlSubMenu->addAction(def.translatedName());
        action->setCheckable(true);
        action->setData(def.name());
        hlActionGroup->addAction(action);

        if (def.name() == m_highlighter->definition().name())
            action->setChecked(true);
    }

    connect(hlActionGroup, &QActionGroup::triggered, this, [this](QAction* action) {
        m_highlighter->setDefinition(m_repository->definitionForName(action->data().toString()));
    });

    // theme selection
    QActionGroup* themeGroup = new QActionGroup(menu.get());
    themeGroup->setExclusive(true);

    QMenu* themeMenu = menu->addMenu(QStringLiteral("Theme"));
    const QList<KSyntaxHighlighting::Theme> themes = m_repository->themes();

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
        setTheme(m_repository->theme(action->data().toString()));
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

void CodeEditor::sidebarPaintEvent(QPaintEvent* event)
{
    QPainter painter(m_sideBar);
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
            painter.drawText(0, top, m_sideBar->width() - 2 - foldingMarkerSize, fontMetrics().height(), Qt::AlignRight, number);
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
            painter.translate(m_sideBar->width() - foldingMarkerSize, top);
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
    int digits = 1;
    for (int count = blockCount(); count >= 10; ++digits, count /= 10);
    return 4 + fontMetrics().horizontalAdvance('9') * digits + fontMetrics().lineSpacing();
}

void CodeEditor::updateSidebarArea(const QRect& rect, int dy)
{
    if (dy)
        m_sideBar->scroll(0, dy);
    else
        m_sideBar->update(0, rect.y(), m_sideBar->width(), rect.height());
}

void CodeEditor::updateSidebarGeometry()
{
    setViewportMargins(sidebarWidth(), 0, 0, 0);
    const QRect r = contentsRect();
    m_sideBar->setGeometry(r.left(), r.top(), sidebarWidth(), r.height());
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
