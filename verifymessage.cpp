#include "verifymessage.h"
#include "fileutilities.h"
#include "treemanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QFileDialog>

VerifyMessage::VerifyMessage(QWidget *parent) : QDialog(parent),
    mTotalMessages(0), mSucceedMessages(0), mFailMessages(0)
{
    initView();
}

// private sections
void VerifyMessage::initView() {
    QVBoxLayout *wholeLayout = new QVBoxLayout;

    QHBoxLayout *fileLayout = new QHBoxLayout;
    QLabel *fileLabel = new  QLabel(tr("File Name:"), this);
    mFileNameEdit = new QLineEdit(this);
    mFileSelectButton = new QPushButton(tr("Select"), this);
    connect(mFileSelectButton, &QAbstractButton::clicked, this, &VerifyMessage::selectFile);
    fileLayout->addWidget(fileLabel);
    fileLayout->addWidget(mFileNameEdit);
    fileLayout->addWidget(mFileSelectButton);
    wholeLayout->addLayout(fileLayout);

    mVerifyButton = new QPushButton(tr("Verify"), this);
    connect(mVerifyButton, &QAbstractButton::clicked, this, &VerifyMessage::verify);
    wholeLayout->addWidget(mVerifyButton);

    QHBoxLayout *resultLayout = new QHBoxLayout;
    resultLayout->addStretch();
    QLabel *totalLabel = new QLabel(tr("Total:"), this);
    mTotalLabel = new QLabel("0", this);
    QLabel *succeedLabel = new QLabel(tr("Succeed:"), this);
    mSucceedLabel = new QLabel("0", this);
    QLabel *failLabel = new QLabel(tr("Fail:"), this);
    mFailLabel = new QLabel("0", this);
    resultLayout->addWidget(totalLabel);
    resultLayout->addWidget(mTotalLabel);
    resultLayout->addWidget(succeedLabel);
    resultLayout->addWidget(mSucceedLabel);
    resultLayout->addWidget(failLabel);
    resultLayout->addWidget(mFailLabel);
    resultLayout->addStretch();
    wholeLayout->addLayout(resultLayout);

    QLabel *failMessageLabel = new QLabel(tr("Fail Messages:"), this);
    wholeLayout->addWidget(failMessageLabel);

    mFailMessageEdit = new QTextEdit(this);
    wholeLayout->addWidget(mFailMessageEdit);

    setLayout(wholeLayout);
}

void VerifyMessage::selectFile() {
    qDebug() << "Select file";

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "/Users/willwu/Documents", "Message Files(*.csv)");

    if(!fileName.isEmpty()) {
        mFileNameEdit->setText(fileName);
    }
}

void VerifyMessage::verify() {
    qDebug() << "Verify messages";

    QStringList messages = FileUtilities::readFromCVSFile(mFileNameEdit->text());

    if(!messages.isEmpty()) {
        TreeManager treeManager(this);
        QList<Element*> elements;
        QString leftString;
        QString failMessages = QString();
        for(int i = 0; i < messages.size(); i++) {
            // 开始验证一条新的信息，一定要先初始化elements and leftString
            elements.clear();
            leftString = QString();
            mTotalMessages++;
            if(!treeManager.findTemplate(messages[i], elements, leftString)) {
                mFailMessages++;
                qDebug() << "Fail message: " << messages[i];
                failMessages += messages[i] + "\n";
            }
            else {
                mSucceedMessages++;
            }
        }

        if(!failMessages.isEmpty()) {
            mFailMessageEdit->setText(failMessages);
        }

        mTotalLabel->setText(QString::number(mTotalMessages));
        mSucceedLabel->setText(QString::number(mSucceedMessages));
        mFailLabel->setText(QString::number(mFailMessages));
    }
}
