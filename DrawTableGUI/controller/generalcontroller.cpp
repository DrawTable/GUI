#include "generalcontroller.h"
#include "pencontroller.h"

GeneralController::GeneralController(Table* view)
{
    this->view = view;
    this->drawController = PenController::getInstance();
    this->pen = new QPen(Qt::black);
}

GeneralController::~GeneralController()
{

}


void GeneralController::mouseDoubleClickEvent(QMouseEvent* event){
    drawController->mouseDoubleClickEvent(view->scene(), event);
}

void GeneralController::mouseMoveEvent(QMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        drawController->mouseMoveEvent(view->scene(), event);
    }
}

void GeneralController::mousePressEvent(QMouseEvent* event){
    drawController->mousePressEvent(view->scene(), event);
}

void GeneralController::mouseReleaseEvent(QMouseEvent* event){
    lastActions.append(drawController->mouseReleaseEvent(view->scene(), event));
}

void GeneralController::setPen(QPen* pen){
    this->pen = pen;
}

QPen* GeneralController::getPen(){
    return pen;
}

void GeneralController::setBrush(QBrush* brush){
    this->brush = brush;
}

QBrush* GeneralController::getBrush(){
    return brush;
}


void GeneralController::setDrawController(AbstractController* drawController){
    this->drawController = drawController;
}

AbstractController* GeneralController::getDrawController(){
    return this->drawController;
}
