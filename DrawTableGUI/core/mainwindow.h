#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QMenu>
#include "../view/table.h"
#include "../controller/generalcontroller.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

private slots:
    void updateToolBarActions(QAction* action);
    void onCursorTriggered(bool checked = false);
    void onPenTriggered(bool checked = false);
    void onDashTriggered(bool checked = false);
    void onEraserTriggered(bool checked = false);
    void onEllipseTriggered(bool checked = false);
    void onRectangleTriggered(bool checked = false);
    void onSaveTriggered();
    void onOpenTriggered();
    void onNewTriggered();
    void onPrintTriggered();
    void onUndoTriggered();
    void onRedoTriggered();

private:
    QMenu* menu;
    QMenu* edit;
    QToolBar* toolBar;
    Table* table;

    QAction* newImg;
    QAction* open;
    QAction* save;
    QAction* print;
    QAction* quit;
    QAction* cursor;
    QAction* pen;
    QAction* dash;
    QAction* eraser;
    QAction* ellipse;
    QAction* rectangle;
    QAction* undo;
    QAction* redo;

    GeneralController* controller;
};

#endif // MAINWINDOW_H
