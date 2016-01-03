#ifndef INPUT_H
#define INPUT_H

#include <QLineEdit>
#include "virtualkeyboard.h"

class VirtualKeyboard;

class Input : public QLineEdit {
    Q_OBJECT

    VirtualKeyboard* keyboard;

protected:
    virtual void focusInEvent(QFocusEvent *e);

public:
    explicit Input(QWidget *parent = 0);
    void setKeyboard(VirtualKeyboard* keyboard);
};

#endif // INPUT_H
