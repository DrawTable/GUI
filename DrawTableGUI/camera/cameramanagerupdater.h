#ifndef CAMERAMANAGERUPDATER_H
#define CAMERAMANAGERUPDATER_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QObject>

#include <QDebug>

class CameraManagerUpdater : public QObject
{
    Q_OBJECT
public:
    CameraManagerUpdater(std::vector<cv::VideoCapture*> captures);
    void stopUpdater();

private:
    std::vector<cv::VideoCapture*> captures;
    bool stopped = false;

public slots:
    void process();

signals:
    void finished();
};

#endif // CAMERAMANAGERUPDATER_H
