#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QString>
#include <QHBoxLayout>
#include <QLabel>

#include "node.h"

namespace Ui {
class MainWindow;
}

class ParseTextEdit;
class NodeEditor;
class TreeManager;

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
    QHBoxLayout *mResultLayout;
    ParseTextEdit *mParseTextEdit;
    NodeEditor *mNodeEditor;
    TreeManager *mTreeManager;
};

#endif // MAINWINDOW_H
