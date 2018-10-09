#include "element.h"

Element::Element()
{

}

// Public Section
void Element::setPickWord(const QString& pickword) {
    this->mPickWord = pickword;
}

QString Element::getPickWord() {
    return mPickWord;
}

void Element::setContent(const QString& content) {
    this->mContent = content;
}

QString Element::getContent() {
    return mContent;
}
