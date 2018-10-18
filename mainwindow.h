#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QString>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QLabel>

#include "node.h"

namespace Ui {
class MainWindow;
}

class ParseTextEdit;
class NodeEditor;
class TreeManager;
class VerifyMessage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void initView();
    int tryToFindTemplate(QString& leftString);
    void initMenuBar();

    void clearAll(QBoxLayout *layout);

private slots:
    void sourceTextChanged();
    void flagTriggered(int type, const QString& selected);
    void addNode(Node* node);
    void saveToFile();

    void classifyAction();
    void verifyAction();

private:
    Ui::MainWindow *ui;
    QTextEdit *mSourceTextEdit;
    QLabel *mElementsLabel;
    //QBoxLayout *mResultLayout;
    QLabel *mResultLabel;
    ParseTextEdit *mParseTextEdit;
    NodeEditor *mNodeEditor;
    TreeManager *mTreeManager;
    VerifyMessage *mVerifyMessageWidget;
    QString mResultString;
};

#endif // MAINWINDOW_H
