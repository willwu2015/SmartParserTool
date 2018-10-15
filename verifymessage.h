#ifndef VERIFYMESSAGE_H
#define VERIFYMESSAGE_H

#include <QDialog>
//#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QStringList>

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
    void verifySingle();

private:
    QLineEdit *mFileNameEdit;
    QPushButton *mFileSelectButton;
    QPushButton *mVerifyButton;
    QPushButton *mVerifySingleButton;
    QLabel *mTotalLabel;
    QLabel *mSucceedLabel;
    QLabel *mFailLabel;
    QTextEdit *mFailMessageEdit;
    QLabel *mFailMessageLabel;
    int mTotalMessages;
    int mSucceedMessages;
    int mFailMessages;
    QStringList mMessages;
    int mCurrentIndex;
};

#endif // VERIFYMESSAGE_H
