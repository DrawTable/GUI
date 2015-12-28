#include "trackingmanager.h"
#include <QDebug>

TrackingManager::TrackingManager(QObject *parent) : QObject(parent)
{

}

void TrackingManager::process() {
    // TODO start the tracking and send data via a signal

    emit finished();
}
