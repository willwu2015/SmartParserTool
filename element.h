#ifndef ELEMENT_H
#define ELEMENT_H

#include <QString>

class Element
{
public:
    Element();

    void setPickWord(const QString& pickword);
    QString getPickWord();
    void setContent(const QString& content);
    QString getContent();

private:
    QString mPickWord;
    QString mContent;
};

#endif // ELEMENT_H
