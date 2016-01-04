#ifndef OPENFILEDIALOG_H
#define OPENFILEDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListView>
#include <QFileSystemModel>
#include "../keyboard/virtualkeyboard.h"
#include "../keyboard/input.h"

/**
 * @brief Dialog window
 *
 * An OpenFileDialog window is shown when the user tries to open an existing drawing.
 * It will also handle the usage of a VirtualKeyboard allowing the user to type the name of the file with the stylus.
 */
class OpenFileDialog : public QDialog
{
    Q_OBJECT

    QLabel* title;
    QLabel* path;
    Input* input;
    QPushButton* open;
    QPushButton* cancel;
    QPushButton* drive;
    VirtualKeyboard* keyboard;
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
