#ifndef HEADNODE_H
#define HEADNODE_H

#include <QString>
#include <QList>
#include <QJsonObject>

class Node;
class Element;

class HeadNode
{
public:
    HeadNode(const QString head);

    void addChild(Node* node);
    Node* findInChildrens(Node* node);
    QString getHeadWord() { return mHeadWord; }
    void upVersionNumber();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    bool match(const QString& source, QList<Element*>& elements, QString& leftString, Node* currentNode);
    Node* matchInChildrens(const QString& source, QList<Element*>& elements, QString& leftString);

private:
    QString mHeadWord;
    int mVersionNumber = 0;
    int mType;
    QList<Node*> mChildrens;
};

#endif // HEADNODE_H
