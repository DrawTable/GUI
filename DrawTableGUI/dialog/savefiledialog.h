#ifndef SAVEFILEDIALOG_H
#define SAVEFILEDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListView>
#include <QFileSystemModel>
#include "../keyboard/virtualkeyboard.h"
#include "../keyboard/input.h"

class SaveFileDialog : public QDialog {
    Q_OBJECT

    QLabel* title;
    QLabel* path;
    Input* input;
    QPushButton* save;
    QPushButton* cancel;
    QPushButton* drive;
    VirtualKeyboard* keyboard;
    QFileSystemModel* model;
    QListView* view;

private slots:
    void onViewClicked(QModelIndex index);
    void onSaveClicked();
    void onCancelClicked();
    void onDriveClicked();

public:
    explicit SaveFileDialog(QWidget *parent = 0);
};

#endif // SAVEFILEDIALOG_H
