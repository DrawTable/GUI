#ifndef KEYMODEL_H
#define KEYMODEL_H

#include <QString>

/**
 * @brief The model of a button for the VirtualKeyboard
 */
struct KeyModel {
    QString letter;
    QString symbol;
    KeyModel(QString letter, QString symbol) {
        this->letter = letter;
        this->symbol = symbol;
    }
};

#endif // KEYMODEL_H
