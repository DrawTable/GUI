#include "generalcontroller.h"
#include "pencontroller.h"
#include <QDebug>

GeneralController::GeneralController(Table* view) {
    this->view = view;
    this->drawController = PenController::getInstance();
    this->pen = new QPen(Qt::white);
    this->view->setController(this);
}

GeneralController::~GeneralController() {
}

void GeneralController::enable() {
    isEnable = true;
}

void GeneralController::disable() {
    isEnable = false;
}

void GeneralController::undo() {
    if (canUndo()) {
        QGraphicsItem* temp = lastActions.takeLast();
        nextActions.append(temp);
        view->scene()->removeItem(temp);
    }
}

void GeneralController::redo() {
    if(canRedo()){
        QGraphicsItem* temp = nextActions.takeLast();
        lastActions.append(temp);
        view->scene()->addItem(temp);
    }
}

void GeneralController::mouseDoubleClickEvent(QMouseEvent* event) {
    if (isEnable) {
        drawController->mouseDoubleClickEvent(view->scene(), event);
    }
}

void GeneralController::mouseMoveEvent(QMouseEvent* event) {
    if (isEnable && event->buttons() == Qt::LeftButton) {
        drawController->mouseMoveEvent(view->scene(), event);
    }
}

void GeneralController::mousePressEvent(QMouseEvent* event) {
    if (isEnable) {
        drawController->mousePressEvent(view->scene(), event, pen);
    }
}

void GeneralController::mouseReleaseEvent(QMouseEvent* event) {
    if (isEnable) {
        lastActions.append(drawController->mouseReleaseEvent(view->scene(), event));
        nextActions.clear();
    }
}

bool GeneralController::canRedo(){
    return !nextActions.isEmpty();
}

bool GeneralController::canUndo(){
    return !lastActions.isEmpty();
}

//------------------------------------------------------------
//------------------- GETTERS AND SETTERS --------------------
//------------------------------------------------------------

void GeneralController::setPen(QPen* pen) {
    this->pen = pen;
}

QPen* GeneralController::getPen() {
    return pen;
}

void GeneralController::setBrush(QBrush* brush) {
    this->brush = brush;
}

QBrush* GeneralController::getBrush() {
    return brush;
}

void GeneralController::setDrawController(AbstractController* drawController) {
    this->drawController = drawController;
}

AbstractController* GeneralController::getDrawController() {
    return drawController;
}
