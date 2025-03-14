/*
    SPDX-FileCopyrightText: 2016 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#pragma once
#include "interface/types/marshal/decompilationinfo.h"
#include <QPlainTextEdit>

namespace KSyntaxHighlighting { class Definition; class SyntaxHighlighter; class Theme; }

class CodeEditorSidebar;
class FindBar;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
    friend class CodeEditorSidebar;
public:
    explicit CodeEditor(QWidget* parent = nullptr);
    void setTabWidth(int width);
    void setText(const QString& text, DisplayLanguage language);
    void setTheme(const KSyntaxHighlighting::Theme& theme);
    void updateSidebarGeometry();

    FindBar* findBar() const { return m_findBar; }
    CodeEditorSidebar* sidebar() const { return m_sidebar; }
protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
private:
    FindBar* m_findBar;
    KSyntaxHighlighting::SyntaxHighlighter* m_highlighter;
    CodeEditorSidebar* m_sidebar;

    void highlightCurrentLine();

    void sidebarPaintEvent(QPaintEvent* event);
    int sidebarWidth() const;
    void updateSidebarArea(const QRect& rect, int dy);

    QTextBlock blockAtPosition(int y) const;
    bool isFoldable(const QTextBlock& block) const;
    bool isFolded(const QTextBlock& block) const;
    void toggleFold(const QTextBlock& block);
};
