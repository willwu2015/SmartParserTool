#include "treemanager.h"
#include "node.h"
#include "headnode.h"
#include "spglobal.h"
#include "element.h"
#include "datafactory.h"
#include "traindata.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFileInfo>
#include <QMessageBox>

TreeManager::TreeManager(QObject *parent) : QObject(parent), mHeadNode(NULL), mCurrentNode(NULL), mTreeModified(false), mMaxId(0)
{

}

/************************************************************************************
 * 有两种情况会调用这个接口
 * 1、新建模版的时候，点击了Flag Head菜单，此时newHeadNode == true
 * 2、在解析新的短信时，会根据短信内容匹配模版，此时newHeadNode == false
 *
 * **********************************************************************************/
void TreeManager::flagHead(const QString& head, bool newHeadNode) {
    if(mHeadNode == NULL) {
        if(loadTreeFromFile(head) == RESULT_SUCCEED) {

        }
        else {
            if(newHeadNode) {
                mHeadNode = new HeadNode(head);
                mTreeModified = true;
            }
        }
    }
    else {
        if(mHeadNode->getHeadWord() != head) {
            loadTreeFromFile(head);
        }
    }

    mCurrentNode = NULL;
}

bool TreeManager::findTemplate(const QString& source, QList<Element*>& elements, QString& leftString) {
    int index = source.indexOf("】");
    QString headString;
    if(index != -1) {
        headString = source.mid(1, index - 1);
    }
    else {
        index = source.indexOf("]");
        if(index != -1) {
            headString = source.mid(1, index - 1);
        }
        else {
            QMessageBox::information(NULL, tr("Information"), tr("Can not find 】or ]. The string is invalid."));
            return RESULT_FAIL_PARSE;
        }
    }

    flagHead(headString);

    QString contentString = source.mid(index + 1);
    qDebug() << "Content string is " << contentString;

    if(mHeadNode != NULL) {
        mCurrentNode = mHeadNode->matchInChildrens(contentString, elements, leftString);
        if(mCurrentNode == NULL) {
            // 解析失败
            return false;
        }
        else {
            if(leftString.isEmpty()) {
                // 完美匹配，模版树到了叶子节点，同时短信也解析完成
                TrainData* baseData = (TrainData*)DataFactory::createData(headString, elements);
                if(baseData != NULL) {
                    baseData->convert();
                }

                return true;
            }
            else {
                // 模版树到了叶子节点，但是短信还没有解析完成，认为解析失败，触发新到模版树生长流程
                return false;
            }
        }
    }
    else {
        return false;
    }
}

void TreeManager::addNode(Node* node) {
    if(node != NULL) {
        if(mCurrentNode != NULL) {
            Node *child = mCurrentNode->findInChildrens(node);

            if(child != NULL) {
                mCurrentNode = child;
            }
            else {
                setNodeId(node);
                node->setParent(mCurrentNode);
                mCurrentNode->addChild(node);
                mCurrentNode = node;
                mTreeModified = true;
            }
        }
        else {
            // 只有headnode
            Node *child = mHeadNode->findInChildrens(node);
            if(child != NULL) { //这个新增的节点已经添加到HeadNode的孩子节点了，不做添加动作
                mCurrentNode = child;
            }
            else { //这是个新的节点，链接到HeadNode下
                setNodeId(node);
                node->setParent(mCurrentNode);
                mHeadNode->addChild(node);
                mCurrentNode = node;
                mTreeModified = true;
            }
        }
    }
}

int TreeManager::saveTreeToFile() {

    QString filename = mHeadNode->getHeadWord() + ".json";
    qDebug() << "File name is " << filename;
    QFile saveFile(filename);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return RESULT_FAIL_OPEN_FILE;
    }

    QJsonObject treeObject;

    QJsonObject headObject;
    mHeadNode->write(headObject);

    treeObject["Head"] = headObject;

    QJsonDocument saveDoc(treeObject);
    qDebug() << "Json is " << saveDoc.toJson();
    saveFile.write(saveDoc.toJson());

    return RESULT_SUCCEED;
}

int TreeManager::loadTreeFromFile(const QString& head) {
    QFile loadFile(head + ".json");

    if(!loadFile.exists()) {
        qDebug() << head + ".json" << " does not exist.";
        return RESULT_NO_FILE;
    }

    if(!loadFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Fail to open " << head + ".json";
        return RESULT_FAIL_OPEN_FILE;
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &jsonError));

    if(jsonError.error != QJsonParseError::NoError) {
        qDebug() << "Fail to farse json. Error is " << jsonError.error;
        return RESULT_FAIL_PARSE;
    }

    QJsonObject rootObject = jsonDoc.object();
    QJsonObject headObject = rootObject.value("Head").toObject();
    QString headString = headObject["HeadWord"].toString();
    mHeadNode = new HeadNode(headString);
    mHeadNode->read(headObject);

    return RESULT_SUCCEED;
}

void TreeManager::setNodeId(Node *node) {
    node->setId(mMaxId);
    mMaxId++;
    qDebug() << "Current Max Id: " << mMaxId;
}
