#include "filedialog.h"

QString FileDialog::fileName;

QString FileDialog::getSaveFileName() {
    SaveFileDialog dialog;
    dialog.exec();
    return fileName;
}

QString FileDialog::getOpenFileName() {
    OpenFileDialog dialog;
    dialog.exec();
    return fileName;
}
