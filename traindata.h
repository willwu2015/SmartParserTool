#ifndef TRAINDATA_H
#define TRAINDATA_H

#include <QObject>
#include <QList>

#include "basedata.h"

class Element;

// 根据铁路客服模版解析出的数据
class TrainData : public BaseData
{
public:
    TrainData(QList<Element*>& elements);

    void convert();

private:
    struct Train {
        int year; //根据订票的月份来判断，如果大于当前月，则和当前年一样，如果小于当前月，则下一年
        int month = -1;
        int day;
        QString ticketId;
        QString name;
        QString trainNumber;
        QString seatNumber;
        QString startStation;
        QString startTime;
        QString checkGate;
        QString returnState;
        bool returnTicket;
    };

    QList<Train*> mTrains;
};

#endif // TRAINDATA_H
