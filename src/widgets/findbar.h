#pragma once
#include <QPlainTextEdit>

namespace Ui {
class FindBar;
}

class FindBar : public QWidget
{
    Q_OBJECT
public:
    explicit FindBar(QPlainTextEdit* parent);
    ~FindBar();

    void setReveal(bool reveal);
private:
    int currentIndex{};
    QList<QPair<int, int>> matches;
    bool textEditChanged{};
    Ui::FindBar* ui;

    void clearMatches();
    void goToMatch(QPair<int, int> range);
private slots:
    void goToNext();
    void goToPrevious();
    void initiateSearch(const QString& searchText);
    void jumpToCurrentMatch();
    void markTextEditChanged();
    void returnPressed();
};

struct FindBarEventFilter : QObject
{
    explicit FindBarEventFilter(FindBar* parent)
        : QObject(parent) {}
    bool eventFilter(QObject*, QEvent* event) override;
};
