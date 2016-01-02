#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QString>
#include "savefiledialog.h"
#include "openfiledialog.h"

class FileDialog {
    friend class SaveFileDialog;
    friend class OpenFileDialog;

    static QString fileName;

public:
    static QString getSaveFileName();
    static QString getOpenFileName();
};

#endif // FILEDIALOG_H
