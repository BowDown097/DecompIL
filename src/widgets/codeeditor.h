/*
    SPDX-FileCopyrightText: 2016 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#pragma once
#include <mutex>
#include <QPlainTextEdit>

namespace KSyntaxHighlighting { class Repository; class SyntaxHighlighter; class Theme; }

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

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
    friend class CodeEditorSidebar;
public:
    explicit CodeEditor(QWidget* parent = nullptr);
    void setText(const QString& text, const QString& defName);
protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
private:
    inline static KSyntaxHighlighting::Repository* m_repository;
    inline static std::once_flag m_onceFlag;

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
