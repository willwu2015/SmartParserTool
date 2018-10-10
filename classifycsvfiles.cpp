#include "classifycsvfiles.h"

#include <QDir>
#include <QFileInfoList>
#include <QDebug>

ClassifyCsvFiles::ClassifyCsvFiles(QObject *parent) : QObject(parent)
{

}

bool ClassifyCsvFiles::process(const QString& directory, const QString& keyWord) {

    QDir dir(directory);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        processFile(fileInfo, keyWord);
    }
    return true;
}

bool ClassifyCsvFiles::processFile(const QFileInfo& fileInfo, const QString& extractWord) {

    QString suffix = fileInfo.suffix();
    if(fileInfo.suffix() == "csv") {
        qDebug() << "Process " << fileInfo.fileName();

        QFile file(fileInfo.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Fail to open the file.";
            return false;
        }

        QTextStream in(&file);
        QStringList csvList;
        csvList.clear();
        while (!in.atEnd()) {
            csvList.push_back(in.readLine());
        }

        QStringList itemsList;
        itemsList.clear();
        QString item = QString();
        int indexFirstComma;
        QString message = QString();
        QString keyWord = QString();
        for(int i = 0; i < csvList.size(); i++) {
            item = csvList[i];
            indexFirstComma = item.indexOf(",");
            message = item.mid(indexFirstComma + 1);

            // 去除引号
            if(message.startsWith("\"")) {
                message = message.mid(1);
            }

            if(message.endsWith("\"")) {
                message = message.mid(0, message.length() - 1);
            }

            if(message.startsWith("【") || message.startsWith("[")) {
                int indexForEnd = message.indexOf("】");
                if(indexForEnd != -1) {
                    keyWord = message.mid(1, indexForEnd - 1);
                }
                else {
                    indexForEnd = message.indexOf("]");
                    if(indexForEnd != -1) {
                        keyWord = message.mid(1, indexForEnd - 1);
                    }
                }

                if(!keyWord.isEmpty() && keyWord == extractWord){
                    itemsList.push_back(message);
                }
            }
        }

        file.close();

        // write to file
        if(itemsList.size() > 0) {
            QFile outfile(fileInfo.absolutePath() + "/" + extractWord + ".csv");
            if (!outfile.open(QIODevice::Append | QIODevice::Text))
                return false;

            QTextStream out(&outfile);
            for(int i = 0; i < itemsList.size(); i++) {
                out << itemsList[i] << "\n";
            }

            outfile.close();
        }
    }

    return true;
}

void ClassifyCsvFiles::printInfo(const QStringList& list) {
    for(int i = 0; i < list.size(); i++) {
        qDebug() << list[i];
    }
}
