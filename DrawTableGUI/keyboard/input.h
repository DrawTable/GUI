#ifndef INPUT_H
#define INPUT_H

#include <QLineEdit>
#include "virtualkeyboard.h"

class VirtualKeyboard;

/**
 * @brief An Input with a VirtualKeyboard.
 * This class describes an Input that make a VirtualKeyboard appears when the user clicks on it.
 */
class Input : public QLineEdit
{
    Q_OBJECT

    VirtualKeyboard* keyboard;

protected:
    virtual void focusInEvent(QFocusEvent *e);

public:
    explicit Input(QWidget *parent = 0);
    void setKeyboard(VirtualKeyboard* keyboard);
};

#endif // INPUT_H
