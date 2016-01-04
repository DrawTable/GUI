#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QToolButton>
#include "mainwindow.h"

class MainWindow;

/**
 * @brief Toolbar of the application
 *
 * This is toolbar of the application. It allows the user to:
 *  - Select the pen tool
 *  - Select the eraser tool
 *  - Select the dash tool
 *  - Select the ellipse tool
 *  - Select the rectangle tool
 *  - Select the dash tool
 *  - Change the thickness of the line
 *  - Change the color of the line
 *  - Undo / Redo
 *
 * The ToolBar is created once by the MainWindow.
 *
 */
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
