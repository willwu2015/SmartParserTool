#include "headnode.h"
#include "node.h"
#include "spglobal.h"
#include "element.h"

#include <QJsonArray>
#include <QDebug>

HeadNode::HeadNode(const QString head) : mHeadWord(head), mType(TYPE_HEAD)
{

}

void HeadNode::addChild(Node *node) {
    if(!mChildrens.contains(node)) {
        mChildrens.append(node);
    }
}

Node* HeadNode::findInChildrens(Node *node) {
    Node *child = NULL;
    for(int i = 0; i < mChildrens.count(); i++) {
        child = mChildrens.at(i);
        if(child->equal(node)) {
            return child;
        }
    }

    return NULL;
}

void HeadNode::upVersionNumber() {
    mVersionNumber++;
}

void HeadNode::read(const QJsonObject &json) {
    mVersionNumber = json.value("VersionNumber").toInt();
    mType = json.value("Type").toInt();

    QJsonArray nodesArray = json.value("nodes").toArray();

    for(int i = 0; i < nodesArray.size(); i++) {
        Node *node = new Node();
        if(node != NULL) {
            node->read(nodesArray[i].toObject());
            addChild(node);
        }
    }
}

void HeadNode::write(QJsonObject &json) const {
    json["VersionNumber"] = mVersionNumber;
    json["HeadWord"] = mHeadWord;
    json["Type"] = mType;

    if(mChildrens.count() > 0) {
        QJsonArray nodeArray;
        for(int i = 0; i < mChildrens.count(); i++) {
            QJsonObject nodeObject;
            mChildrens[i]->write(nodeObject);
            nodeArray.append(nodeObject);
        }
        json["nodes"] = nodeArray;
    }
}

/********************************************************************************
 * source：输入参数
 * elements：输出参数，将结果返回
 * leftString：输出参数，将剩余没有解析的字符串返回
 * match算法与Node::match()中的一样，这里修改后需要同步修改那里
 * 后面可以考虑优化
 * *****************************************************************************/
bool HeadNode::match(const QString& source, QList<Element*>& elements, QString& leftString, Node* currentNode) {

    if(source <= END_STRING_LENGTH) {
        // 模版树还没有到叶子节点，但是待解析的短信已经到头了
        return true;
    }

    Node* node = NULL;
    QString keyWord = QString();
    QString pickEnd = QString();
    int indexForKeyWord = 0;
    int indexForPickEnd = 0;
    int indexForContent = 0;
    QString nextSoure = QString();

    for(int i = 0; i < mChildrens.size(); i++) {
        node = mChildrens[i];
        if(node != NULL) {
            keyWord = node->getKeyWord();
            if(!keyWord.isEmpty()) {
                indexForKeyWord = source.indexOf(keyWord);
            }

            pickEnd = node->getPickEnd();
            if(!pickEnd.isEmpty()) {
                indexForPickEnd = source.indexOf(pickEnd);
                if(indexForKeyWord < indexForPickEnd) {
                    Element* element = new Element();
                    if(element != NULL) {
                        indexForContent = indexForKeyWord + keyWord.length();
                        element->setContent(source.mid(indexForContent, indexForPickEnd - indexForContent));
                        element->setPickWord(node->getPickWord());
                        elements.append(element);
                        nextSoure = source.mid(indexForPickEnd + pickEnd.length());
                        if(node->getType() != TYPE_LEAF) {
                            if(!node->match(nextSoure, elements, leftString, currentNode)) {
                                continue;
                            }
                            else {
                                return true;
                            }
                        }
                        else {
                            if(nextSoure.length() > 2) {
                                // 待解析的短信已经到头了，但是模版树已经到叶子节点了，说明模版树需要继续生长
                                qDebug() << "There are more text needed be parsed. (" << nextSoure << ")";
                                leftString = nextSoure;
                                currentNode = node;
                                return false;
                            }
                            return true;
                        }
                    }
                }
                else {
                    continue;
                }
            }
            else {
                qDebug() << "There must be something wrong. PickEnd must not be empty!";
                qDebug() << "Source is " << source;
                return false;
            }
        }
    }

    return false;
}

Node* HeadNode::matchInChildrens(const QString& source, QList<Element*>& elements, QString& leftString) {

    Node* node = NULL;
    QString keyWord = QString();
    QString pickEnd = QString();
    int indexForKeyWord = 0;
    int indexForPickEnd = 0;
    int indexForContent = 0;
    QString nextSoure = QString();
    QString content = QString();
    int maxContentLength = -1;

    for(int i = 0; i < mChildrens.size(); i++) {
        node = mChildrens[i];
        if(node != NULL) {
            keyWord = node->getKeyWord();
            if(!keyWord.isEmpty()) {
                indexForKeyWord = source.indexOf(keyWord);
            }
            else {
                indexForKeyWord = 0;
            }

            pickEnd = node->getPickEnd();
            if(!pickEnd.isEmpty()) {
                if(!node->isRegExpForPickEnd()) {
                    indexForPickEnd = source.indexOf(pickEnd, indexForKeyWord + keyWord.length());
                }
                else {
                    indexForPickEnd = source.indexOf(QRegExp(pickEnd), indexForKeyWord + keyWord.length());
                }

                if(indexForKeyWord >= 0 && (indexForKeyWord < indexForPickEnd)) {
                    Element* element = new Element();
                    if(element != NULL) {
                        indexForContent = indexForKeyWord + keyWord.length();
                        content = source.mid(indexForContent, indexForPickEnd - indexForContent);
                        maxContentLength = node->getMaxContentLength();
                        if(maxContentLength >= 0 && content.length() > maxContentLength) {
                            continue;
                        }
                        element->setContent(content);
                        element->setPickWord(node->getPickWord());
                        elements.append(element);
                        if(node->isRegExpForPickEnd()) {
                            nextSoure = source.mid(indexForPickEnd);
                        }
                        else {
                            nextSoure = source.mid(indexForPickEnd + pickEnd.length());
                        }

                        if(node->getType() != TYPE_LEAF) {
                            if(nextSoure.length() <= END_STRING_LENGTH) {
                                // 虽然模版树没有到叶子，但是待解析的短信已经完成，退出解析
                                return node;
                            }
                            else {
                                node = node->matchInChildrens(nextSoure, elements, leftString);
                                if(node == NULL) {
                                    //在当前节点的子节点中没有匹配的，而且短信也没有解析完，认为解析失败，elements需要清除
                                    elements.clear();
                                    return NULL;
                                }
                                else {
                                    return node;
                                }
                            }
                        }
                        else {
                            if(nextSoure.length() > END_STRING_LENGTH) {
                                // 待解析的短信没有到头了，但是模版树已经到叶子节点了，说明模版树需要继续生长
                                qDebug() << "There are more text needed be parsed. (" << nextSoure << ")";
                                leftString = nextSoure;
                                return node;
                            }
                            return node;
                        }
                    }
                }
                else {
                    continue;
                }
            }
            else {
                qDebug() << "There must be something wrong. PickEnd must not be empty!";
                qDebug() << "Source is " << source;
                return NULL;
            }
        }
    }

    // 在子节点中没有找到，将剩余的字符串返回，说明模版不匹配，将父节点返回，方便后续的树生长
    leftString = source;
    return node->getParent();
}
