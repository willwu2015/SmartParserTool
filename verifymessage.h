#ifndef VERIFYMESSAGE_H
#define VERIFYMESSAGE_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>

class VerifyMessage : public QDialog
{
    Q_OBJECT

public:
    VerifyMessage(QWidget *parent = 0);

private:
    void initView();

private slots:
    void selectFile();
    void verify();

private:
    QLineEdit *mFileNameEdit;
    QPushButton *mFileSelectButton;
    QPushButton *mVerifyButton;
    QLabel *mTotalLabel;
    QLabel *mSucceedLabel;
    QLabel *mFailLabel;
    QTextEdit *mFailMessageEdit;
    int mTotalMessages;
    int mSucceedMessages;
    int mFailMessages;
};

#endif // VERIFYMESSAGE_H
