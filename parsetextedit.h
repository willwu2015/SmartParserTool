#ifndef PARSETEXTEDIT_H
#define PARSETEXTEDIT_H

#include<QTextEdit>

class ParseTextEdit : public QTextEdit
{
    Q_OBJECT
public: // const define
    const static int TYPE_HEAD = 0;
    const static int TYPE_KEYWORD = 1;
    const static int TYPE_PICKEND = 2;
    const static int TYPE_CONTENT = 3;

private: // const define
    const static int STATE_HEAD = 0;
    const static int STATE_KEYWORD = 1;
    const static int STATE_PICKEND = 2;

public:
    explicit ParseTextEdit(QWidget *parent = nullptr);

    void init(bool fromHead);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void flagTriggered(int type, const QString& selected);

public slots:

private slots:
    void flagHead();
    void flagKeyWord();
    void flagPickContent();
    void flagPickEnd();
    void nullKeyWord();

private:
    void stateChangedTo(int state);

private:
    int mCurrentFlagIndex = 0;
    int mState = STATE_HEAD;
};

#endif // PARSETEXTEDIT_H
