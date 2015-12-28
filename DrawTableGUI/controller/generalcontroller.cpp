#include "generalcontroller.h"
#include "pencontroller.h"
#include <QDebug>

GeneralController::GeneralController(Table* view) {
    this->view = view;
    this->drawController = PenController::getInstance();
    this->pen = new QPen(Qt::black);
    this->view->setController(this);
}

GeneralController::~GeneralController() {
}

void GeneralController::undo() {
    if (canUndo()) {
        QGraphicsItem* temp = lastActions.takeLast();
        nextActions.append(temp);
        view->scene()->removeItem(temp);
        modifToSave = true;
    }
}

void GeneralController::redo() {
    if(canRedo()){
        QGraphicsItem* temp = nextActions.takeLast();
        lastActions.append(temp);
        view->scene()->addItem(temp);
        modifToSave = true;
    }
}

void GeneralController::mouseDoubleClickEvent(QMouseEvent* event) {
    drawController->mouseDoubleClickEvent(view->scene(), event);
}

void GeneralController::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() == Qt::LeftButton) {
        drawController->mouseMoveEvent(view->scene(), event);
    }
}

void GeneralController::mousePressEvent(QMouseEvent* event) {
    drawController->mousePressEvent(view->scene(), event, pen);
}

void GeneralController::mouseReleaseEvent(QMouseEvent* event) {
    lastActions.append(drawController->mouseReleaseEvent(view->scene(), event));
    nextActions.clear();
    modifToSave = true;
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

bool GeneralController::toSave()
{
    return modifToSave;
}

void GeneralController::setToSave(bool toSave)
{
    this->modifToSave = toSave;
}
