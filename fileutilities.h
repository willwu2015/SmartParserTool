#ifndef FILEUTILITIES_H
#define FILEUTILITIES_H

#include <QStringList>

class FileUtilities
{
public:
    FileUtilities();

    static QStringList readFromCVSFile(const QString& fileName);
    static QString preProcess(const QString& message);
};

#endif // FILEUTILITIES_H
