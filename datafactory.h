#ifndef DATAFACTORY_H
#define DATAFACTORY_H

#include <QList>
#include "basedata.h"

class Element;

class DataFactory
{
public:
    DataFactory();

    static BaseData* createData(const QString& keyWord, QList<Element*>& elements);
};

#endif // DATAFACTORY_H
