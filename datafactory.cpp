#include "datafactory.h"
#include "traindata.h"

DataFactory::DataFactory()
{

}

BaseData* DataFactory::createData(const QString& keyWord, QList<Element*>& elements) {
    if(keyWord == "铁路客服") {
        return new TrainData(elements);
    }

    return NULL;
}
