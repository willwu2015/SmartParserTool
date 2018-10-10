#include "traindata.h"
#include "spglobal.h"
#include "element.h"

#include <QString>
#include <QRegExp>

TrainData::TrainData(QList<Element*>& elements)
{
    mDataType = DATA_TYPE_TRAIN;
    mRawData = elements;
}

void TrainData::convert() {

    if(mRawData.size() > 0) {
        Train* train = new Train();
        Train* oldTrain;
        QString pickWord = QString();
        QRegExp timeRegExp("0[0-9]:[0-5][0-9]|1[0-9]:[0-5][0-9]|2[0-3]:[0-5][0-9]");
        for(int i = 0; i < mRawData.size(); i++) {
            pickWord = mRawData[i]->getPickWord();
            if(pickWord == "订单号") {
                train->ticketId = mRawData[i]->getContent();
            }
            else if(pickWord == "姓名") {
                train->name = mRawData[i]->getContent();
            }
            else if(pickWord == "月") {
                if(train->month > 0) {
                    // month初始是-1，如果大于0，说明这不是第一张票，需要重新new一个Train，并把原来的压入list中
                    mTrains.append(train);
                    oldTrain = train;
                    train = new Train();
                    train->ticketId = oldTrain->ticketId;
                    train->name = oldTrain->name;

                }

                train->month = (mRawData[i]->getContent()).toInt();
            }
            else if(pickWord == "日") {
                train->day = (mRawData[i]->getContent()).toInt();
            }
            else if(pickWord == "车次") {
                train->trainNumber = mRawData[i]->getContent();
            }
            else if(pickWord == "座位号") {
                train->seatNumber = mRawData[i]->getContent();
            }
            else if(pickWord == "始发站") {
                train->startStation = mRawData[i]->getContent();
            }
            else if(pickWord == "始发时间") {
                train->startTime = mRawData[i]->getContent();
            }
            else if(pickWord == "检票口") {
                train->checkGate = mRawData[i]->getContent();
            }
            else if(pickWord == "座位号始发站始发时间") {
                // 需要进一步切分，比如：7车16C号、16D号三亚16:25   2车6D号、6F号海口东9:15
                QString pickContent = mRawData[i]->getContent();
                int indexForTime = pickContent.indexOf(timeRegExp);
                if(indexForTime != -1) {
                    train->startTime = pickContent.mid(indexForTime);
                }

                int indexForStation = pickContent.lastIndexOf("号");
                if(indexForStation != -1) {
                    train->startStation = pickContent.mid(indexForStation + 1, indexForTime - indexForStation - 1);
                    train->seatNumber = pickContent.mid(0, indexForStation + 1);
                }
            }
            else if(pickWord == "退票状态") {
                train->returnTicket = true;
                train->returnState = mRawData[i]->getContent();
            }
            else if(pickWord == "流水号") {
                train->ticketId = mRawData[i]->getContent();
            }
        }
        mTrains.append(train);
    }
}
