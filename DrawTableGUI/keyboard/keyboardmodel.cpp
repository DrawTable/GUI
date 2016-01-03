#include "keyboardmodel.h"

const QList<KeyModel> KeyboardModel::FIRST_ROW = QList<KeyModel>()
        << KeyModel("q", "1") << KeyModel("w", "2") << KeyModel("e", "3") << KeyModel("r", "4") << KeyModel("t", "5")
        << KeyModel("z", "6") << KeyModel("u", "7") << KeyModel("i", "8") << KeyModel("o", "9") << KeyModel("p", "0");

const QList<KeyModel> KeyboardModel::SECOND_ROW = QList<KeyModel>()
        << KeyModel("a", "!") << KeyModel("s", "@") << KeyModel("d", "#") << KeyModel("f", "$") << KeyModel("g", "%")
        << KeyModel("h", "&&") << KeyModel("j", "*") << KeyModel("k", "?") << KeyModel("l", "/");

const QList<KeyModel> KeyboardModel::THIRD_ROW = QList<KeyModel>()
        << KeyModel("y", "_") << KeyModel("x", "\"") << KeyModel("c", "'") << KeyModel("v", "(") << KeyModel("b", ")")
        << KeyModel("n", "-") << KeyModel("m", "+");

const QList<KeyModel> KeyboardModel::FOURTH_ROW = QList<KeyModel>()
        << KeyModel(",", ",") << KeyModel(" ", " ") << KeyModel(".", ".");
