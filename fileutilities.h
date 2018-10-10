#ifndef FILEUTILITIES_H
#define FILEUTILITIES_H

#include <QStringList>

class FileUtilities
{
public:
    FileUtilities();

    static QStringList readFromCVSFile(const QString& fileName);
};

#endif // FILEUTILITIES_H
