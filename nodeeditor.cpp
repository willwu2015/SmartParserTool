#include "nodeeditor.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>

#include "node.h"

// Public Section
NodeEditor::NodeEditor(QWidget *parent) : QWidget(parent)
{
    initView();
}

void NodeEditor::setKeyWord(const QString& keyword) {
    mKeyWordEditor->setText(keyword);
}

void NodeEditor::setContent(const QString& content) {
    mContentEditor->setText(content);
}

void NodeEditor::setPickEnd(const QString& pickend) {
    mPickEndEditor->setText(pickend);
}

// Private Section
void NodeEditor::initView() {

    QVBoxLayout *totalLayout = new QVBoxLayout(this);
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("Node"));
    totalLayout->addWidget(group);

    QVBoxLayout *layout = new QVBoxLayout(group);

    QHBoxLayout *keyworkLayout = new QHBoxLayout;
    QLabel *keywordLabel = new QLabel(this);
    keywordLabel->setText(tr("Keyword:"));
    mKeyWordEditor = new QLineEdit(this);
    mKeyWordEditor->setReadOnly(true);
    keyworkLayout->addWidget(keywordLabel);
    keyworkLayout->addWidget(mKeyWordEditor);
    layout->addLayout(keyworkLayout);

    QHBoxLayout *pickWordLayout = new QHBoxLayout;
    QLabel *pickWordLabel = new QLabel(this);
    pickWordLabel->setText(tr("Pick Word:"));
    mPickWordEditor = new QLineEdit(this);
    pickWordLayout->addWidget(pickWordLabel);
    pickWordLayout->addWidget(mPickWordEditor);
    layout->addLayout(pickWordLayout);

    QHBoxLayout *pickEndLayout = new QHBoxLayout;
    QLabel *pickEndLabel = new QLabel(this);
    pickEndLabel->setText(tr("Pick End:"));
    mPickEndEditor = new QLineEdit(this);
    pickEndLayout->addWidget(pickEndLabel);
    pickEndLayout->addWidget(mPickEndEditor);
    layout->addLayout(pickEndLayout);

    mIsRegExp = new QCheckBox(tr("This is a regular expression."), this);
    layout->addWidget(mIsRegExp);

    QHBoxLayout *contentLayout = new QHBoxLayout;
    QLabel *contentLabel = new QLabel(this);
    contentLabel->setText(tr("Content:"));
    mContentEditor = new QLineEdit(this);
    mContentEditor->setReadOnly(true);
    QLabel *contentLengthLabel = new QLabel(tr("Max length:"), this);
    mMaxContentLength = new QLineEdit(this);
    contentLayout->addWidget(contentLabel);
    contentLayout->addWidget(mContentEditor);
    contentLayout->addWidget(contentLengthLabel);
    contentLayout->addWidget(mMaxContentLength);
    layout->addLayout(contentLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    mAddButton = new QPushButton(this);
    mAddButton->setText(tr("Add"));
    connect(mAddButton, &QPushButton::clicked, this, &NodeEditor::addButtonPushed);
    buttonLayout->addStretch();
    buttonLayout->addWidget(mAddButton);
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);

    group->setLayout(layout);
}

void NodeEditor::addButtonPushed() {
    Node *node = new Node(mKeyWordEditor->text(), mPickWordEditor->text(), mPickEndEditor->text(), mIsRegExp->checkState() == Qt::Checked);
    if(mMaxContentLength->text().isEmpty()) {
        node->setMaxContentLength(-1);
    }
    else {
        node->setMaxContentLength(mMaxContentLength->text().toInt());
    }

    emit addNode(node);
}
