#include "toolbar.h"

ToolBar::ToolBar(MainWindow* parent) : QToolBar(parent) {

    /* actions creation */

    menu = new QAction(this);
    menu->setIcon(QIcon(":/tool/icons/menu.png"));
    connect(menu, SIGNAL(triggered()), this, SLOT(onMenuTriggered()));

    pen = new QAction(this);
    pen->setCheckable(true);
    pen->setChecked(true);
    pen->setIcon(QIcon(":/tool/icons/pen.png"));
    connect(pen, SIGNAL(triggered(bool)), this, SLOT(onPenTriggered(bool)));

    undo = new QAction(this);
    undo->setIcon(QIcon(":/tool/icons/undo.png"));
    connect(undo, SIGNAL(triggered()), this, SLOT(onUndoTriggered()));

    redo = new QAction(this);
    redo->setIcon(QIcon(":/tool/icons/redo.png"));
    connect(redo, SIGNAL(triggered()), this, SLOT(onRedoTriggered()));

    dash = new QAction(this);
    dash->setCheckable(true);
    dash->setIcon(QIcon(":/tool/icons/dash.png"));
    connect(dash, SIGNAL(triggered(bool)), this, SLOT(onDashTriggered(bool)));

    eraser = new QAction(this);
    eraser->setCheckable(true);
    eraser->setIcon(QIcon(":/tool/icons/eraser.png"));
    connect(eraser, SIGNAL(triggered(bool)), this, SLOT(onEraserTriggered(bool)));

    ellipse = new QAction(this);
    ellipse->setCheckable(true);
    ellipse->setIcon(QIcon(":/tool/icons/ellipse.png"));
    connect(ellipse, SIGNAL(triggered(bool)), this, SLOT(onEllipseTriggered(bool)));

    rectangle = new QAction(this);
    rectangle->setCheckable(true);
    rectangle->setIcon(QIcon(":/tool/icons/rectangle.png"));
    connect(rectangle, SIGNAL(triggered(bool)), this, SLOT(onRectangleTriggered(bool)));

    color = new QAction(this);
    color->setIcon(QIcon(":/tool/icons/color.png"));
    connect(color, SIGNAL(triggered()), this, SLOT(onColorTriggered()));

    QList<QIcon> icons;
    for (int i = 0; i < 6; ++i) {
        icons << QIcon(QString(":/tool/icons/thickness%1.png").arg(i + 1));
    }

    QStringList thicknesses;
    thicknesses << "1 px"  << "3 px" << "5 px"  << "7 px" << "10 px" << "15 px";

    QMenu* thicknessMenu = new QMenu(this);
    for (int i = 0; i < thicknesses.count(); ++i) {
        QAction *action = new QAction(icons.at(i), thicknesses.at(i), this);
        action->setData(thicknesses.at(i).split(" ")[0].toInt());
        action->setCheckable(true);
        connect(action, SIGNAL(triggered()), this, SLOT(onThicknessChanged()));
        thicknessMenu->addAction(action);
    }
    thicknessMenu->actions().first()->setChecked(true);
    thicknessMenu->setStyleSheet("background: rgb(46,46,46);");

    thickness = new QToolButton(this);
    thickness->setIcon(QIcon(":/tool/icons/thickness.png"));
    thickness->setPopupMode(QToolButton::InstantPopup);
    thickness->setMenu(thicknessMenu);
    thickness->setStyleSheet("QToolButton::menu-indicator{image: none;}");

    /* actions integration */

    QWidget* spacer1 = new QWidget();
    spacer1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QWidget* spacer2 = new QWidget();
    spacer2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    addAction(menu);
    addWidget(spacer1);
    addAction(pen);
    addAction(eraser);
    addSeparator();
    addWidget(thickness);
    addSeparator();
    addAction(undo);
    addAction(redo);
    addSeparator();
    addAction(dash);
    addAction(ellipse);
    addAction(rectangle);
    addSeparator();
    addAction(color);
    addWidget(spacer2);

    /* toolbar customisation */

    setFloatable(false);
    setMovable(false);
    setStyleSheet("QToolBar{ background: rgb(46,46,46); border: 0px; }");
}

void ToolBar::onThicknessChanged() {
    const QList<QAction*>& actions = thickness->menu()->actions();
    foreach (QAction* a, actions) {
        a->setChecked(false);
    }
    QAction* action = qobject_cast<QAction*>(sender());
    action->setChecked(true);

    MainWindow* window = qobject_cast<MainWindow*>(parent());
    if (!window) { return; }
    window->onThicknessChanged(action);
}

void ToolBar::onPenTriggered(bool checked) {
    MainWindow* window = qobject_cast<MainWindow*>(parent());
    if (!window) { return; }
    window->onPenTriggered(checked);
}

void ToolBar::onDashTriggered(bool checked) {
    MainWindow* window = qobject_cast<MainWindow*>(parent());
    if (!window) { return; }
    window->onDashTriggered(checked);
}

void ToolBar::onEraserTriggered(bool checked) {
    MainWindow* window = qobject_cast<MainWindow*>(parent());
    if (!window) { return; }
    window->onEraserTriggered(checked);
}

void ToolBar::onEllipseTriggered(bool checked) {
    MainWindow* window = qobject_cast<MainWindow*>(parent());
    if (!window) { return; }
    window->onEllipseTriggered(checked);
}

void ToolBar::onRectangleTriggered(bool checked) {
    MainWindow* window = qobject_cast<MainWindow*>(parent());
    if (!window) { return; }
    window->onRectangleTriggered(checked);
}

void ToolBar::onMenuTriggered() {
    MainWindow* window = qobject_cast<MainWindow*>(parent());
    if (!window) { return; }
    window->onMenuTriggered();
}

void ToolBar::onUndoTriggered() {
    MainWindow* window = qobject_cast<MainWindow*>(parent());
    if (!window) { return; }
    window->onUndoTriggered();
}

void ToolBar::onRedoTriggered() {
    MainWindow* window = qobject_cast<MainWindow*>(parent());
    if (!window) { return; }
    window->onRedoTriggered();
}

void ToolBar::onColorTriggered(){
    MainWindow* window = qobject_cast<MainWindow*>(parent());
    if (!window) { return; }
    window->onColorTriggered();
}

QToolButton* ToolBar::getThickness() {
    return thickness;
}

QAction* ToolBar::getMenu() {
    return menu;
}

QAction* ToolBar::getPen() {
    return pen;
}

QAction* ToolBar::getDash() {
    return dash;
}

QAction* ToolBar::getEraser() {
    return eraser;
}

QAction* ToolBar::getEllipse() {
    return ellipse;
}

QAction* ToolBar::getRectangle() {
    return rectangle;
}

QAction* ToolBar::getUndo() {
    return undo;
}

QAction* ToolBar::getRedo() {
    return redo;
}

QAction* ToolBar::getColor() {
    return color;
}
