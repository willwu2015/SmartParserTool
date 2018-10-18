#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QList>
#include <QFileDialog>
#include <QInputDialog>

#include "parsetextedit.h"
#include "nodeeditor.h"
#include "treemanager.h"
#include "spglobal.h"
#include "element.h"
#include "classifycsvfiles.h"
#include "verifymessage.h"
#include "fileutilities.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), mTreeManager(new TreeManager()),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initMenuBar();
    initView();
    mResultString = QString();
}

MainWindow::~MainWindow()
{
    delete ui;

    if(mTreeManager != NULL) {
        delete mTreeManager;
        mTreeManager = NULL;
    }
}

int MainWindow::tryToFindTemplate(QString& leftString) {
    QString source = mSourceTextEdit->toPlainText();

    source = FileUtilities::preProcess(source);
    QList<Element*> elements;
    if(mTreeManager->findTemplate(source, elements, leftString)) {
        // 将elements内容显示在
        Element* element = NULL;
        QString result = QString();
        for(int i = 0; i < elements.size(); i++) {
            element = elements[i];
            if(element != NULL) {
                result += element->getPickWord() + "/" + element->getContent() + "\n";
            }
        }

        mElementsLabel->setText(result);
        return RESULT_SUCCEED;
    }
    else {
        return RESULT_FAIL_FIND_TEMPLATE;
    }
}

// Private Sections
void MainWindow::initView() {
    QWidget *centralWidget = ui->centralWidget;

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QHBoxLayout *sourceLayout = new QHBoxLayout;
    QLabel *sourceLabel = new QLabel(centralWidget);
    sourceLabel->setText(tr("Source:"));
    mSourceTextEdit = new QTextEdit(centralWidget);
    connect(mSourceTextEdit, SIGNAL(textChanged()), this, SLOT(sourceTextChanged()));
    sourceLayout->addWidget(sourceLabel);
    sourceLayout->addWidget(mSourceTextEdit);
    layout->addLayout(sourceLayout);

    QHBoxLayout *elementsLayout = new QHBoxLayout;
    QLabel *elementsLabel = new QLabel(centralWidget);
    elementsLabel->setText(tr("Parse Result:"));
    mElementsLabel = new QLabel(centralWidget);
    mElementsLabel->setWordWrap(true);
    elementsLayout->addWidget(elementsLabel);
    elementsLayout->addWidget(mElementsLabel);
    elementsLayout->addStretch();
    layout->addLayout(elementsLayout);

    QVBoxLayout *parseLayout = new QVBoxLayout;
    //mResultLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    //mResultLayout->addStretch();
    //mResultLayout->setSizeConstraint(QLayout::SetFixedSize);
    //parseLayout->addLayout(mResultLayout);

    mResultLabel = new QLabel(this);
    mResultLabel->setWordWrap(true);
    parseLayout->addWidget(mResultLabel);
    mParseTextEdit = new ParseTextEdit(centralWidget);
    connect(mParseTextEdit, &ParseTextEdit::flagTriggered, this, &MainWindow::flagTriggered);
    parseLayout->addWidget(mParseTextEdit);
    layout->addLayout(parseLayout);

    mNodeEditor = new NodeEditor(centralWidget);
    connect(mNodeEditor, &NodeEditor::addNode, this, &MainWindow::addNode);
    layout->addWidget(mNodeEditor);

    QHBoxLayout *saveFileLayout = new QHBoxLayout;
    QPushButton *saveButton = new QPushButton(centralWidget);
    saveButton->setText(tr("Save"));
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveToFile);
    saveFileLayout->addWidget(saveButton);
    layout->addLayout(saveFileLayout);
}

void MainWindow::initMenuBar() {
    QAction* pActionA = new QAction(tr("&Classify"));
    connect(pActionA, &QAction::triggered, this, &MainWindow::classifyAction);

    QAction* pActionB = new QAction(tr("&Verify"));
    connect(pActionB, &QAction::triggered, this, &MainWindow::verifyAction);

    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(pActionA);
    fileMenu->addAction(pActionB);
}

void MainWindow::clearAll(QBoxLayout* layout) {
    if(layout != NULL) {
        qDebug() << "layout count: " << layout->count();
        for(int i = layout->count() - 1; i >= 0; i--) {
            layout->removeItem(layout->itemAt(i));
        }
    }
}

// Private slots
void MainWindow::sourceTextChanged() {
    if(mSourceTextEdit != NULL && mParseTextEdit != NULL) {
        QString sourceString = mSourceTextEdit->toPlainText();
        if(sourceString.startsWith("【") || sourceString.startsWith("[") ||
           sourceString.endsWith("】") || sourceString.endsWith("]")) {
            QString leftString = QString();

            if(tryToFindTemplate(leftString) == RESULT_SUCCEED) {
                mParseTextEdit->setText(tr("Find a template to match."));
            }
            else {
                if(leftString.isEmpty()) {
                    mParseTextEdit->setText(FileUtilities::preProcess(sourceString));
                    mParseTextEdit->init(true);
                }
                else {
                    mParseTextEdit->setText(leftString);
                    mParseTextEdit->init(false);
                }
            }
        }
        else {
            if(!sourceString.isEmpty()) {
                QMessageBox::information(this, tr("Information"), tr("The message must start with 【 or ["));
            }
        }
    }
}

void MainWindow::flagTriggered(int type, const QString& selected) {
    qDebug() << "Process flag string in MainWindow: " << type <<"/" << selected;
    switch(type) {
    case ParseTextEdit::TYPE_HEAD: {
            QLabel *label = new QLabel(ui->centralWidget);
            label->setStyleSheet("color:red;");
            label->setText(selected);
            //qDebug() << "result layout count: " << mResultLayout->count();
            mResultString += "<font color=\"red\">" + selected + " ";
            //mResultLayout->insertWidget(mResultLayout->count() - 1, label);
            mResultLabel->setText(mResultString);

            if(mTreeManager != NULL) {
                mTreeManager->flagHead(selected, true);
            }
        }
        break;

    case ParseTextEdit::TYPE_KEYWORD: {
            QLabel *label = new QLabel(ui->centralWidget);
            label->setStyleSheet("color:red;");
            label->setText(selected);
            //qDebug() << "result layout count: " << mResultLayout->count();
            mResultString += "<font color=\"red\">" + selected + " ";
            //mResultLayout->insertWidget(mResultLayout->count() - 1, label);
            mResultLabel->setText(mResultString);

            mNodeEditor->setKeyWord(selected);
        }

        break;

    case ParseTextEdit::TYPE_PICKEND: {
            QLabel *label = new QLabel(ui->centralWidget);
            label->setStyleSheet("color:red;");
            label->setText(selected);
            //qDebug() << "result layout count: " << mResultLayout->count();
            mResultString += "<font color=\"red\">" + selected + " ";
            //mResultLayout->insertWidget(mResultLayout->count() - 1, label);
            mResultLabel->setText(mResultString);

            mNodeEditor->setPickEnd(selected);
        }
        break;

    case ParseTextEdit::TYPE_CONTENT: {
            QLabel *label = new QLabel(ui->centralWidget);
            label->setStyleSheet("color:blue;");
            label->setText(selected);
            //qDebug() << "result layout count: " << mResultLayout->count();
            mResultString += "<font color=\"blue\">" + selected + " ";
            //mResultLayout->insertWidget(mResultLayout->count() - 1, label);
            mResultLabel->setText(mResultString);

            mNodeEditor->setContent(selected);
        }
        break;

    default:
        qDebug() << "The type " << type << "can not be matched.";
    }

}

void MainWindow::addNode(Node* node) {
    if(mTreeManager != NULL) {
        mTreeManager->addNode(node);
    }
}

void MainWindow::saveToFile() {
    int result = mTreeManager->saveTreeToFile();
    qDebug() << "The result of saving tree to file is " << result;
}

void MainWindow::classifyAction() {
    qDebug() << "Classify action is triggered.";

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                      "/Users/willwu/Documents",
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);

    qDebug() << "Process files in " << dir;

    // 获取提取关键字
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                               tr("Key Word:"), QLineEdit::Normal,
                                               QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty()) {
        if(ClassifyCsvFiles::process(dir, text)) {
            qDebug() << "Succeed to process csv files.";
        }
    }
}

void MainWindow::verifyAction() {
//    mVerifyMessageWidget = new VerifyMessage(ui->centralWidget);
    //mVerifyMessageWidget->show();
    VerifyMessage verifyMessageDialog;
    verifyMessageDialog.exec();
}
