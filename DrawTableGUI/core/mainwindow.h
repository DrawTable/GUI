#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QPixmap>
#include "../view/table.h"

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

private:
    QMenu* menu;
    Table* table;
    QAction* open;
    QAction* save;
    QAction* quit;
    QAction* cursor;
    QAction* pen;
    QAction* dash;
    QAction* eraser;
    QAction* ellipse;
    QAction* rectangle;
    QToolBar* toolBar;
};

#endif // MAINWINDOW_H
