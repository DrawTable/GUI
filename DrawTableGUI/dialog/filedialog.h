#ifndef SYSTEMFILEDIALOG_H
#define SYSTEMFILEDIALOG_H

#include <QString>
#include "savefiledialog.h"
#include "openfiledialog.h"

class SystemFileDialog
{
    friend class SaveFileDialog;
    friend class OpenFileDialog;

    static QString fileName;

public:
    static QString getSaveFileName();
    static QString getOpenFileName();
};

#endif // SYSTEMFILEDIALOG_H
