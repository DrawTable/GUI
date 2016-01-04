#ifndef MENU_H
#define MENU_H

#include "../core/mainwindow.h"
#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>

/**
 * @brief Menu of the application
 *
 * This is the main menu of the application. It allows the user to:
 *  - Create a new drawing
 *  - Open an existing drawing
 *  - Save a drawing
 *  - Print the drawing
 *  - Quit the application
 *
 * The Menu is created once by the MainWindow.
 *
 */
class Menu : public QDialog {
    Q_OBJECT

    class MenuItem : public QPushButton {
    public:
        MenuItem(QString pictureOn, QString pictureOff, QWidget* parent = nullptr);
    };

public:
    Menu(MainWindow* parent = 0);
    ~Menu();

private slots:
    void onNewClicked();
    void onOpenClicked();
    void onSaveClicked();
    void onPrintClicked();
    void onQuitClicked();

private:
    MenuItem* _new;
    MenuItem* open;
    MenuItem* save;
    MenuItem* print;
    MenuItem* quit;
    QHBoxLayout* layout;
};

#endif // MENU_H
