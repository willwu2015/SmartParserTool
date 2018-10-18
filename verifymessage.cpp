#include "verifymessage.h"
#include "fileutilities.h"
#include "treemanager.h"
#include "element.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QFileDialog>

VerifyMessage::VerifyMessage(QWidget *parent) : QDialog(parent),
    mTotalMessages(0), mSucceedMessages(0), mFailMessages(0), mMessages(QStringList()), mCurrentIndex(0)
{
    initView();
}

// private sections
void VerifyMessage::initView() {
    QVBoxLayout *wholeLayout = new QVBoxLayout(this);
    wholeLayout->setSizeConstraint(QLayout::SetFixedSize);

    QHBoxLayout *fileLayout = new QHBoxLayout;
    QLabel *fileLabel = new  QLabel(tr("File Name:"), this);
    mFileNameEdit = new QLineEdit(this);
    mFileSelectButton = new QPushButton(tr("Select"), this);
    connect(mFileSelectButton, &QAbstractButton::clicked, this, &VerifyMessage::selectFile);
    fileLayout->addWidget(fileLabel);
    fileLayout->addWidget(mFileNameEdit);
    fileLayout->addWidget(mFileSelectButton);
    wholeLayout->addLayout(fileLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    mVerifyButton = new QPushButton(tr("Verify"), this);
    connect(mVerifyButton, &QAbstractButton::clicked, this, &VerifyMessage::verify);
    mVerifySingleButton = new QPushButton(tr("Verify Next"), this);
    connect(mVerifySingleButton, &QAbstractButton::clicked, this, &VerifyMessage::verifySingle);
    buttonLayout->addWidget(mVerifyButton);
    buttonLayout->addWidget(mVerifySingleButton);
    wholeLayout->addLayout(buttonLayout);

    QHBoxLayout *resultLayout = new QHBoxLayout;
    resultLayout->addStretch();
    QLabel *totalLabel = new QLabel(tr("Total:"), this);
    mTotalLabel = new QLabel("0", this);
    QLabel *succeedLabel = new QLabel(tr("Succeed:"), this);
    mSucceedLabel = new QLabel("0", this);
    QLabel *failLabel = new QLabel(tr("Fail:"), this);
    mFailLabel = new QLabel("0", this);
    mFailLabel->setWordWrap(true);
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

    mFailMessageLabel = new QLabel(this);
    mFailMessageLabel->setVisible(false);
    mFailMessageLabel->setWordWrap(true);
    mFailMessageEdit = new QTextEdit(this);
    wholeLayout->addWidget(mFailMessageLabel);
    wholeLayout->addWidget(mFailMessageEdit);
    wholeLayout->addStretch();

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
            if(!treeManager.findTemplate(FileUtilities::preProcess(messages[i]), elements, leftString)) {
                mFailMessages++;
                qDebug() << "Fail message: " << messages[i];
                failMessages += messages[i] + "\n";
            }
            else {
                mSucceedMessages++;
            }
        }

        if(!failMessages.isEmpty()) {
            if(!mFailMessageEdit->isVisible()) {
                mFailMessageLabel->setVisible(false);
                mFailMessageEdit->setVisible(true);
            }
            mFailMessageEdit->setText(failMessages);
        }

        mTotalLabel->setText(QString::number(mTotalMessages));
        mSucceedLabel->setText(QString::number(mSucceedMessages));
        mFailLabel->setText(QString::number(mFailMessages));
    }
}

void VerifyMessage::verifySingle() {
    if(mMessages.isEmpty()) {
        mMessages = FileUtilities::readFromCVSFile(mFileNameEdit->text());
        mCurrentIndex = 0;
    }

    QString message = QString();
    if(mCurrentIndex < mMessages.size()) {
        message = mMessages[mCurrentIndex];
        message = FileUtilities::preProcess(message);
    }
    else {
        return;
    }

    QList<Element*> elements;
    QString leftString;
    TreeManager treeManager(this);

    if(!mFailMessageLabel->isVisible()) {
        mFailMessageLabel->setVisible(true);
        mFailMessageEdit->setVisible(false);
    }

    if(treeManager.findTemplate(message, elements, leftString)) {
        QString result = mMessages[mCurrentIndex] + "\n";
        for(int i = 0; i < elements.size(); i++) {
            result += elements[i]->getPickWord() + "/" + elements[i]->getContent() + "\n";
        }
        mFailMessageLabel->setText(result);
    }
    else {
        mFailMessageLabel->setText(mMessages[mCurrentIndex]);
    }

    mCurrentIndex++;
}
