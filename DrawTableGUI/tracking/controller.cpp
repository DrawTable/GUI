#include "controller.h"

Controller::Controller()
{
    Worker* worker = new Worker;
    worker->moveToThread(&thread);

    connect(&thread, SIGNAL(started()), worker, SLOT(doWork()));
    connect(worker, SIGNAL(finished()), &thread, SLOT(quit()));
    connect(&thread, SIGNAL(finished()), worker, SLOT(deleteLater()));

    connect(this, SIGNAL(mouseMove(int,int)), worker, SLOT(mouseMouve(int,int)));
    connect(this, SIGNAL(mousePressed()), worker, SLOT(mousePressed()));
    connect(this, SIGNAL(mouseReleased()), worker, SLOT(mouseReleased()));
}

void Controller::start()
{
    thread.start();
}
