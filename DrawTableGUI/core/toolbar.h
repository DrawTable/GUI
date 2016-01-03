#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QToolButton>
#include "mainwindow.h"

class MainWindow;

class ToolBar : public QToolBar {
    Q_OBJECT

    QToolButton* thickness;
    QAction* menu;
    QAction* pen;
    QAction* dash;
    QAction* eraser;
    QAction* ellipse;
    QAction* rectangle;
    QAction* undo;
    QAction* redo;
    QAction* color;

public:
    explicit ToolBar(MainWindow* parent = 0);
    QToolButton* getThickness();
    QAction* getMenu();
    QAction* getPen();
    QAction* getDash();
    QAction* getEraser();
    QAction* getEllipse();
    QAction* getRectangle();
    QAction* getUndo();
    QAction* getRedo();
    QAction* getColor();

public slots:
    void onMenuTriggered();
    void onPenTriggered(bool checked = false);
    void onDashTriggered(bool checked = false);
    void onEraserTriggered(bool checked = false);
    void onEllipseTriggered(bool checked = false);
    void onRectangleTriggered(bool checked = false);
    void onColorTriggered();
    void onUndoTriggered();
    void onRedoTriggered();
    void onThicknessChanged();
};

#endif // TOOLBAR_H
