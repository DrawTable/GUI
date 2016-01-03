#include "virtualkeyboard.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QDesktopWidget>

VirtualKeyboard::VirtualKeyboard(Input *input, QWidget *parent) : QWidget(parent), input(input) {

    input->setKeyboard(this);
    setFixedHeight(parent->height() / 2);

    QHBoxLayout* firstRow = new QHBoxLayout();
    addModelToRow(firstRow, KeyboardModel::FIRST_ROW);

    QHBoxLayout* secondRow = new QHBoxLayout();
    addModelToRow(secondRow, KeyboardModel::SECOND_ROW);

    QHBoxLayout* thirdRow = new QHBoxLayout();
    shift = new KeyboardButton();
    shift->setText(QString::fromUtf8("SHIFT"));
    shift->setFixedHeight(height() / 5);
    connect(shift, SIGNAL(clicked(bool)), this, SLOT(onShiftClicked()));
    thirdRow->addWidget(shift);

    addModelToRow(thirdRow, KeyboardModel::THIRD_ROW);

    backspace = new KeyboardButton();
    backspace->setText(QString::fromUtf8("DEL"));
    backspace->setFixedHeight(height() / 5);
    connect(backspace, SIGNAL(clicked(bool)), this, SLOT(onBackspaceClicked()));
    thirdRow->addWidget(backspace);

    QHBoxLayout* fourthRow = new QHBoxLayout();
    _hide = new KeyboardButton();
    _hide->setText(QString::fromUtf8("HIDE"));
    _hide->setFixedHeight(height() / 5);
    connect(_hide, SIGNAL(clicked(bool)), this, SLOT(hide()));
    fourthRow->addWidget(_hide);

    addModelToRow(fourthRow, KeyboardModel::FOURTH_ROW);

    alphanum = new KeyButton(KeyModel("ABC", "12#"));
    alphanum->setFixedHeight(height() / 5);
    connect(alphanum, SIGNAL(clicked(bool)), this, SLOT(onAlphanumClicked()));
    fourthRow->addWidget(alphanum);

    enter = new KeyboardButton();
    enter->setText("Enter");
    enter->setFixedHeight(height() / 5);
    connect(enter, SIGNAL(clicked(bool)), this, SLOT(onEnterClicked()));
    fourthRow->addWidget(enter);

    QVBoxLayout* rowsLayout = new QVBoxLayout();
    QHBoxLayout* hlayout;

    hlayout= new QHBoxLayout();
    hlayout->addStretch();
    rowsLayout->addLayout(firstRow);
    hlayout->addStretch();
    rowsLayout->addLayout(hlayout);

    hlayout= new QHBoxLayout();
    hlayout->addStretch();
    rowsLayout->addLayout(secondRow);
    hlayout->addStretch();
    rowsLayout->addLayout(hlayout);

    hlayout= new QHBoxLayout();
    hlayout->addStretch();
    rowsLayout->addLayout(thirdRow);
    hlayout->addStretch();
    rowsLayout->addLayout(hlayout);

    hlayout= new QHBoxLayout();
    hlayout->addStretch();
    rowsLayout->addLayout(fourthRow);
    hlayout->addStretch();
    rowsLayout->addLayout(hlayout);

    shiftModifier = false;
    symbolModifier = false;

    setLayout(rowsLayout);
    setStyleSheet("background: black; border: 0px;");
}

VirtualKeyboard::~VirtualKeyboard() {

}

void VirtualKeyboard::addModelToRow(QHBoxLayout* rowLayout, const QList<KeyModel>& rowModel) {
    foreach (KeyModel model, rowModel) {
        KeyButton* key = new KeyButton(model);
        key->setFixedHeight(height() / 5);
        connect(key, SIGNAL(clicked(bool)), this, SLOT(onKeyClicked()));
        keys.append(key);
        rowLayout->addWidget(key);
    }
}

void VirtualKeyboard::onKeyClicked() {
    KeyButton* key = qobject_cast<KeyButton*>(sender());
    if (!key) { return; }
    input->setText(input->text().append(key->text()));
    input->setFocus();
}

void VirtualKeyboard::onShiftClicked() {
    shiftModifier = !shiftModifier;
    foreach (KeyButton* key, keys) { key->shift(shiftModifier); }
}

void VirtualKeyboard::onBackspaceClicked() {
    QString s = input->text();
    s.truncate(s.size() -1);
    input->setText(s);
    input->setFocus();
}

void VirtualKeyboard::onAlphanumClicked() {
    symbolModifier = !symbolModifier;
    foreach (KeyButton* key, keys) { key->invert(symbolModifier); }
    alphanum->invert(symbolModifier);
}

void VirtualKeyboard::onEnterClicked() {
    emit enterClicked();
}
