#ifndef NODEEDITOR_H
#define NODEEDITOR_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

class Node;

class NodeEditor : public QWidget
{
    Q_OBJECT
public:
    explicit NodeEditor(QWidget *parent = nullptr);

    void setKeyWord(const QString& keyword);
    void setContent(const QString& content);
    void setPickEnd(const QString& pickend);

signals:
    void addNode(Node* node);

public slots:

private:
    void initView();

private slots:
    void addButtonPushed();

private:
    QLineEdit *mKeyWordEditor;
    QLineEdit *mPickWordEditor;
    QLineEdit *mPickEndEditor;
    QCheckBox *mIsRegExp;
    QLineEdit *mContentEditor;
    QLineEdit *mMaxContentLength;
    QPushButton *mAddButton;
};

#endif // NODEEDITOR_H
