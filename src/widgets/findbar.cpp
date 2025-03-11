#include "findbar.h"
#include "ui_findbar.h"
#include <QScrollBar>

bool FindBarEventFilter::eventFilter(QObject*, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        FindBar* findBar = qobject_cast<FindBar*>(parent());
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->key() == Qt::Key_Return && findBar->isVisible())
            return true;
        else if (keyEvent->modifiers() & Qt::ControlModifier && keyEvent->key() == Qt::Key_F && findBar->isHidden())
            findBar->setReveal(true);
        else if (keyEvent->key() == Qt::Key_Escape && findBar->isVisible())
            findBar->setReveal(false);
    }

    return false;
}

FindBar::FindBar(QPlainTextEdit* parent)
    : QWidget(parent), ui(new Ui::FindBar)
{
    setFont(qApp->font()); // don't inherit font of TextEdit

    ui->setupUi(this);
    ui->matchesLabel->hide();
    parent->installEventFilter(new FindBarEventFilter(this));

    hide();

    connect(parent, &QPlainTextEdit::textChanged, this, &FindBar::markTextEditChanged);
    connect(ui->closeButton, &QToolButton::clicked, this, [this] { setReveal(false); });
    connect(ui->nextButton, &QToolButton::clicked, this, &FindBar::goToNext);
    connect(ui->previousButton, &QToolButton::clicked, this, &FindBar::goToPrevious);
    connect(ui->searchBox, &QLineEdit::returnPressed, this, &FindBar::returnPressed);
    connect(ui->searchBox, &QLineEdit::textChanged, this, &FindBar::initiateSearch);
}

FindBar::~FindBar()
{
    delete ui;
}

void FindBar::clearMatches()
{
    matches.clear();
    ui->matchesLabel->hide();
    ui->nextButton->setEnabled(false);
    ui->previousButton->setEnabled(false);
    currentIndex = 0;
}

void FindBar::goToMatch(QPair<int, int> range)
{
    QPlainTextEdit* parentTextEdit = qobject_cast<QPlainTextEdit*>(parent());

    QTextCursor textCursor = parentTextEdit->textCursor();
    textCursor.setPosition(range.first, QTextCursor::MoveAnchor);
    textCursor.setPosition(range.second, QTextCursor::KeepAnchor);

    parentTextEdit->setTextCursor(textCursor);
}

void FindBar::goToNext()
{
    currentIndex++;
    jumpToCurrentMatch();
}

void FindBar::goToPrevious()
{
    currentIndex--;
    jumpToCurrentMatch();
}

void FindBar::initiateSearch(const QString& searchText)
{
    clearMatches();
    if (searchText.isEmpty())
        return;

    QString plainText = qobject_cast<QPlainTextEdit*>(parent())->toPlainText();
    qsizetype ind{};

    while ((ind = plainText.indexOf(searchText, ind, Qt::CaseInsensitive)) != -1)
    {
        matches.append(qMakePair(ind, ind + searchText.size()));
        ind += searchText.size();
    }

    jumpToCurrentMatch();
}

void FindBar::jumpToCurrentMatch()
{
    if (textEditChanged)
    {
        textEditChanged = false;
        initiateSearch(ui->searchBox->text());
    }

    ui->matchesLabel->show();
    ui->nextButton->setEnabled(currentIndex + 1 < matches.size());
    ui->previousButton->setEnabled(currentIndex >= 1);

    if (matches.isEmpty())
    {
        ui->matchesLabel->setText(tr("Phrase not found"));
        return;
    }

    goToMatch(matches[currentIndex]);
    ui->matchesLabel->setText(matches.size() > 1
        ? tr("%1 of %2 matches").arg(currentIndex + 1).arg(matches.size())
        : tr("%1 of 1 match").arg(currentIndex + 1));
}

void FindBar::markTextEditChanged()
{
    if (ui->matchesLabel->isVisible())
        textEditChanged = true;
}

void FindBar::returnPressed()
{
    if (matches.isEmpty())
        return;

    if (++currentIndex >= matches.size())
        currentIndex = 0;

    jumpToCurrentMatch();
}

void FindBar::setReveal(bool reveal)
{
    if (reveal)
    {
        QPlainTextEdit* parentTextEdit = qobject_cast<QPlainTextEdit*>(parent());
        move(parentTextEdit->width() - parentTextEdit->verticalScrollBar()->width() - 250, 0);
        show();
        ui->searchBox->setFocus();
    }
    else
    {
        clearMatches();
        hide();
        ui->searchBox->clear();
    }
}
