#ifndef KEYBOARDMODEL_H
#define KEYBOARDMODEL_H

#include <QList>
#include "keymodel.h"

class KeyboardModel {

public:
    const static QList<KeyModel> FIRST_ROW;
    const static QList<KeyModel> SECOND_ROW;
    const static QList<KeyModel> THIRD_ROW;
    const static QList<KeyModel> FOURTH_ROW;
};

#endif // KEYBOARDMODEL_H
