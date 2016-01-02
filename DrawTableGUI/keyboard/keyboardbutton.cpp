#include "keyboardbutton.h"

KeyboardButton::KeyboardButton() : QPushButton() {
    setStyleSheet("QPushButton{background: #35322f; color: white;} QPushButton::hover{border: 2px solid white;}");
}

KeyboardButton::~KeyboardButton() {

}
