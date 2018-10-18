#include "parsetextedit.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QDebug>

/***************************************************************
 * 将一段文本按照：HEAD KEYWORD CONTENT PICKEND （KEYWORD） CONTENT PICKEND
 * 的顺序进行切割
 * 注意：
 * HEAD只能有一个
 * KEYWORD必须紧贴着前序词，也可以为空
 * CONTENT：是自动切割的，不需要用户选择
 * 通过这个切割后，把整个文本拆分成一个一个相关内容，没有任何剩余
 * ************************************************************/
ParseTextEdit::ParseTextEdit(QWidget *parent) : QTextEdit(parent)
{

}

void ParseTextEdit::init(bool fromHead) {
    if(fromHead) {
        stateChangedTo(STATE_HEAD);
    }
    else {
        stateChangedTo(STATE_KEYWORD);
    }
    mCurrentFlagIndex = 0;
}

// Protected Section
void ParseTextEdit::contextMenuEvent(QContextMenuEvent *event) {
    QMenu *menu = new QMenu(this);

    // flag head
    QAction *flagHeadAction = new QAction(this);
    flagHeadAction->setText(tr("Flag Head"));
    connect(flagHeadAction, &QAction::triggered, this, &ParseTextEdit::flagHead);
    menu->addAction(flagHeadAction);

    // flag keyword
    QAction *flagKeyWordAction = new QAction(this);
    flagKeyWordAction->setText(tr("Flag Keyword"));
    connect(flagKeyWordAction, &QAction::triggered, this, &ParseTextEdit::flagKeyWord);
    menu->addAction(flagKeyWordAction);

    // insert null keyword
    QAction *nullKeyWordAction = new QAction(this);
    nullKeyWordAction->setText(tr("Insert null keyword"));
    connect(nullKeyWordAction, &QAction::triggered, this, &ParseTextEdit::nullKeyWord);
    menu->addAction(nullKeyWordAction);

    // flag pickend
    QAction *flagPickEndAction = new QAction(this);
    flagPickEndAction->setText(tr("Flag Pickend"));
    connect(flagPickEndAction, &QAction::triggered, this, &ParseTextEdit::flagPickEnd);
    menu->addAction(flagPickEndAction);

    switch(mState) {
    case STATE_HEAD:
        flagHeadAction->setEnabled(true);
        flagKeyWordAction->setEnabled(false);
        nullKeyWordAction->setEnabled(false);
        flagPickEndAction->setEnabled(false);
        break;

    case STATE_KEYWORD:
        flagHeadAction->setEnabled(false);
        flagKeyWordAction->setEnabled(true);
        nullKeyWordAction->setEnabled(true);
        flagPickEndAction->setEnabled(false);
        break;

    case STATE_PICKEND:
        flagHeadAction->setEnabled(false);
        flagKeyWordAction->setEnabled(false);
        nullKeyWordAction->setEnabled(false);
        flagPickEndAction->setEnabled(true);
        break;

    default:
        qDebug() << "The state " << mState << " can not be identified.";
        break;
    }

    menu->exec(event->globalPos());
    delete menu;
}

// Private Section
void ParseTextEdit::flagHead() {
    qDebug() << "flag head is clicked!";
    QString selected = this->textCursor().selectedText();
    QString source = this->toPlainText();

    qDebug() << "Selected text is " << selected;

    int index = source.indexOf(selected);
    qDebug() << "Selected text is indexed at " << index;

    if( index == 1) {
        emit flagTriggered(ParseTextEdit::TYPE_HEAD, this->textCursor().selectedText());
        mCurrentFlagIndex = index + selected.length() + 1;
        qDebug() << "Current index " << mCurrentFlagIndex;
        stateChangedTo(STATE_KEYWORD);
    }
    else if(index > 1) {
        emit flagTriggered(ParseTextEdit::TYPE_HEAD, this->textCursor().selectedText());
        mCurrentFlagIndex = 0;
        qDebug() << "Current index " << mCurrentFlagIndex;
        stateChangedTo(STATE_KEYWORD);
    }
    else {
        qDebug() << "Error: source: " << source;
    }
}

void ParseTextEdit::flagKeyWord() {
    qDebug() << "flag keyword is clicked!";
    QString selected = this->textCursor().selectedText();
    QString source = this->toPlainText();

    qDebug() << "Selected text is " << selected;
    int index = source.indexOf(selected, mCurrentFlagIndex);

    if(index != mCurrentFlagIndex) {
        // pop a dialog to notify user
    }
    else {
        emit flagTriggered(ParseTextEdit::TYPE_KEYWORD, this->textCursor().selectedText());
        mCurrentFlagIndex = index + selected.length();
        qDebug() << "Current index " << mCurrentFlagIndex;
        stateChangedTo(STATE_PICKEND);
    }
}

void ParseTextEdit::nullKeyWord() {
    qDebug() << "insert a null key word";

    emit flagTriggered(ParseTextEdit::TYPE_KEYWORD, "");
    stateChangedTo(STATE_PICKEND);
}

void ParseTextEdit::flagPickContent() {
    qDebug() << "flag pick content is clicked!";
    qDebug() << "Selected text is " << this->textCursor().selectedText();
    emit flagTriggered(ParseTextEdit::TYPE_CONTENT, this->textCursor().selectedText());
}

void ParseTextEdit::flagPickEnd() {
    qDebug() << "flag pick end is clicked!";
    QString selected = this->textCursor().selectedText();
    QString source = this->toPlainText();

    qDebug() << "Selected text is " << selected;

    int index = source.indexOf(selected, mCurrentFlagIndex);
    qDebug() << "pick end, index is " << index;
    QString content = source.mid(mCurrentFlagIndex, index - mCurrentFlagIndex);
    emit flagTriggered(ParseTextEdit::TYPE_CONTENT, content);
    emit flagTriggered(ParseTextEdit::TYPE_PICKEND, this->textCursor().selectedText());

    mCurrentFlagIndex = index + selected.length();
    qDebug() << "Current index " << mCurrentFlagIndex;
    stateChangedTo(STATE_KEYWORD);
}

void ParseTextEdit::stateChangedTo(int state) {
    if(mState != state) {
        mState = state;
    }
}
