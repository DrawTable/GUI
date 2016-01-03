#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QHBoxLayout>
#include <QList>
#include "keybutton.h"
#include "keyboardmodel.h"
#include "input.h"

class Input;

class Keyboard : public QWidget {
    Q_OBJECT

    QList<KeyButton*> keys;
    KeyButton* alphanum;
    KeyboardButton* shift;
    KeyboardButton* backspace;
    KeyboardButton* _hide;
    KeyboardButton* enter;

    Input* input;

    bool shiftModifier;
    bool symbolModifier;

    void addModelToRow(QHBoxLayout* rowLayout, const QList<KeyModel>& rowModel);

private slots:
    void onKeyClicked();
    void onShiftClicked();
    void onBackspaceClicked();
    void onAlphanumClicked();
    void onEnterClicked();

signals:
    enterClicked();

public:
    Keyboard(Input* input, QWidget *parent = 0);
    ~Keyboard();
};

#endif // KEYBOARD_H
