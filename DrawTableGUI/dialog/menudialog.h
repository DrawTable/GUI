#ifndef MENUDIALOG_H
#define MENUDIALOG_H

#include "../core/mainwindow.h"
#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>

class MenuDialog : public QDialog {
    Q_OBJECT

public:
    MenuDialog(MainWindow* parent = 0);
    ~MenuDialog();

private slots:
    void onNewClicked();
    void onOpenClicked();
    void onSaveClicked();
    void onPrintClicked();
    void onQuitClicked();

private:
    QPushButton* _new;
    QPushButton* open;
    QPushButton* save;
    QPushButton* print;
    QPushButton* quit;
    QHBoxLayout* layout;
};

#endif // MENUDIALOG_H
