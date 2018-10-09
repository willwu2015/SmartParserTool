#ifndef TREEMANAGER_H
#define TREEMANAGER_H

#include <QObject>
#include <QString>
#include <QList>

class Node;
class HeadNode;
class Element;

class TreeManager : public QObject
{
    Q_OBJECT
public:
    explicit TreeManager(QObject *parent = nullptr);

    void flagHead(const QString& head, bool newHeadNode = false);
    void addNode(Node* node);
    int saveTreeToFile();

    bool findTemplate(const QString& source, QList<Element*>& elements, QString& leftString);

signals:

public slots:

private:
    int loadTreeFromFile(const QString& head);
    void setNodeId(Node* node);

private:
    HeadNode *mHeadNode;
    Node *mCurrentNode;
    bool mTreeModified;
    int mMaxId;
};

#endif // TREEMANAGER_H
