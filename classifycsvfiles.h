#ifndef CLASSIFYCSVFILES_H
#define CLASSIFYCSVFILES_H

#include <QObject>
#include <QFileInfo>
#include <QStringList>

class ClassifyCsvFiles : public QObject
{
    Q_OBJECT
public:
    explicit ClassifyCsvFiles(QObject *parent = nullptr);

    static bool process(const QString& directory, const QString& keyWord);

signals:

public slots:

private:
    static bool processFile(const QFileInfo& fileInfo, const QString& keyWord);
    static void printInfo(const QStringList& list);
};

#endif // CLASSIFYCSVFILES_H
