#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <QWidget>
#include <QHBoxLayout>
#include <QList>
#include "keybutton.h"
#include "keyboardmodel.h"
#include "input.h"

class Input;

/**
 * @brief The VirtualKeyboard
 *
 * The virtual keyboard allows the user to type some text using just the stylus.
 * You can see it as a smartphone keyboard.
 *
 * It is used when the user tries to open or save a file via the OpenFileDialog and the SaveFileDialog classes.
 */
class VirtualKeyboard : public QWidget {
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
    void enterClicked();

public:
    VirtualKeyboard(Input* input, QWidget *parent = 0);
    ~VirtualKeyboard();
};

#endif // VIRTUALKEYBOARD_H
