#ifndef INPUT_H
#define INPUT_H

#include <QLineEdit>
#include "keyboard.h"

class Keyboard;

class Input : public QLineEdit {
    Q_OBJECT

    Keyboard* keyboard;

protected:
    virtual void focusInEvent(QFocusEvent *e);

public:
    explicit Input(QWidget *parent = 0);
    void setKeyboard(Keyboard* keyboard);
};

#endif // INPUT_H
