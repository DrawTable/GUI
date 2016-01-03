#ifndef KEYBUTTON_H
#define KEYBUTTON_H

#include "keyboardbutton.h"
#include "keymodel.h"


/**
 * @brief A button for the VirtualKeyboard
 */
class KeyButton : public KeyboardButton {
    Q_OBJECT

    KeyModel model;

public:
    explicit KeyButton(KeyModel model);
    void invert(bool modifier);
    void shift(bool modifier);
};

#endif // KEYBUTTON_H
