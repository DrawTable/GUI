#include "keybutton.h"

KeyButton::KeyButton(KeyModel model) : KeyboardButton(), model(model)
{
    setText(model.letter);
}

void KeyButton::invert(bool modifier)
{
    setText(modifier ? model.symbol : model.letter);
}

void KeyButton::shift(bool modifier)
{
    setText(modifier ? text().toUpper() : text().toLower());
}

