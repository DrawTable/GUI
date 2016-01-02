#include "input.h"

Input::Input(QWidget *parent) : QLineEdit(parent) {

}

void Input::focusInEvent(QFocusEvent *e) {
    QLineEdit::focusInEvent(e);
    keyboard->show();
}

void Input::setKeyboard(Keyboard *keyboard) {
    this->keyboard = keyboard;
}
