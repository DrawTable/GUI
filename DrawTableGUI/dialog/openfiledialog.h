#ifndef OPENFILEDIALOG_H
#define OPENFILEDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListView>
#include <QFileSystemModel>
#include "../keyboard/keyboard.h"
#include "../keyboard/input.h"

class OpenFileDialog : public QDialog {
    Q_OBJECT

    QLabel* title;
    QLabel* path;
    Input* input;
    QPushButton* open;
    QPushButton* cancel;
    QPushButton* drive;
    Keyboard* keyboard;

    QFileSystemModel* model;
    QListView* view;

private slots:
    void onViewClicked(QModelIndex index);
    void onOpenClicked();
    void onCancelClicked();
    void onDriveClicked();

public:
    explicit OpenFileDialog(QWidget *parent = 0);
};

#endif // OPENFILEDIALOG_H
