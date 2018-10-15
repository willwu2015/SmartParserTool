#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QList>
#include <QJsonObject>

class Element;

class Node
{
public:
    Node();
    Node(const QString& keyword, const QString& pickword, const QString& pickend, bool isRegExp = false);

    int getId();
    void setId(int id);
    void setParent(Node* node);
    Node* getParent() { return mParent; }
    void addChild(Node* node);
    QString getKeyWord() { return mKeyWord; }
    QString getPickWord() { return mPickWord; }
    QString getPickEnd() { return mPickEnd; }
    bool isRegExpForPickEnd() { return mIsRegExpForPickEnd; }
    Node* findInChildrens(Node *node);
    int getType();
    void setType(int type);
    void setMaxContentLength(int length) { mMaxContentLength = length; }
    int getMaxContentLength() { return mMaxContentLength; }

    bool equal(Node* node);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    bool match(const QString& source, QList<Element*>& elements, QString& leftString, Node* currentNode);
    Node* matchInChildrens(const QString& source, QList<Element*>& elements, QString& leftString);

private:
    int id;
    QString mKeyWord;
    QString mPickWord;
    QString mPickEnd;
    bool mIsRegExpForPickEnd;
    int mType;
    int mMaxContentLength;
    Node* mParent;
    QList<Node*> mChildrens;
};

#endif // NODE_H
