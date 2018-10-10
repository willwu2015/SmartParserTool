#include "fileutilities.h"

#include <QFile>
#include <QDebug>

FileUtilities::FileUtilities()
{

}

QStringList FileUtilities::readFromCVSFile(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Fail to open the file.";
        return QStringList();
    }

    QTextStream in(&file);
    QStringList csvList;
    csvList.clear();
    while (!in.atEnd()) {
        csvList.push_back(in.readLine());
    }
    file.close();

    return csvList;
}
