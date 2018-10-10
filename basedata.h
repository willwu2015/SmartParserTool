#ifndef BASEDATA_H
#define BASEDATA_H

#include <QList>

class Element;

class BaseData
{
public:
    BaseData();

    virtual void convert() = 0;

protected:
    int mDataType;
    QList<Element*> mRawData;
};

#endif // BASEDATA_H
