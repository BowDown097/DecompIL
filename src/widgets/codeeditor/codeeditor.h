/*
    SPDX-FileCopyrightText: 2016 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#pragma once
#include <QPlainTextEdit>

namespace KSyntaxHighlighting { class Definition; class SyntaxHighlighter; class Theme; }

class CodeEditorSidebar;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
    friend class CodeEditorSidebar;
public:
    explicit CodeEditor(QWidget* parent = nullptr);
    void setText(const QString& text, const KSyntaxHighlighting::Definition& definition);
protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
private:
    KSyntaxHighlighting::SyntaxHighlighter* m_highlighter;
    CodeEditorSidebar* m_sideBar;

    void highlightCurrentLine();
    void setTheme(const KSyntaxHighlighting::Theme& theme);

    void sidebarPaintEvent(QPaintEvent* event);
    int sidebarWidth() const;
    void updateSidebarArea(const QRect& rect, int dy);
    void updateSidebarGeometry();

    QTextBlock blockAtPosition(int y) const;
    bool isFoldable(const QTextBlock& block) const;
    bool isFolded(const QTextBlock& block) const;
    void toggleFold(const QTextBlock& block);
};
