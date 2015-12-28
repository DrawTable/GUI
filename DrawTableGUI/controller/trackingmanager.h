#ifndef TRACKINGMANAGER_H
#define TRACKINGMANAGER_H

#include <QObject>

class TrackingManager : public QObject
{
    Q_OBJECT
public:
    explicit TrackingManager(QObject *parent = 0);

public slots:
    void process();

signals:
    void finished();
    void error(QString err);
};

#endif // TRACKINGMANAGER_H
