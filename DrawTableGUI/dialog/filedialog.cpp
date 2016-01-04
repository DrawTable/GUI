#include "filedialog.h"

QString SystemFileDialog::fileName;

QString SystemFileDialog::getSaveFileName()
{
    SaveFileDialog dialog;
    dialog.exec();
    return fileName;
}

QString SystemFileDialog::getOpenFileName()
{
    OpenFileDialog dialog;
    dialog.exec();
    return fileName;
}
