#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QObject>
#include <QThread>

#include <QDebug>

#include "cameramanagerupdater.h"

class CameraManager : public QObject
{
    Q_OBJECT
public:
    static CameraManager* instance;
    static CameraManager* getInstance();

    void listCameras();
    void startUpdater();
    int countCameras();

    static void mouseHandler(int, int, int, int, void*);

private:
    CameraManager();
    std::vector<cv::VideoCapture*> captures;
    void destroyWindows();
    QThread* thread;
    CameraManagerUpdater* updater;
    void onUpdaterFinished();
    void emitCameraId(int cameraId);

signals:
    void cameraChoosen(int id);
};

#endif // CAMERAMANAGER_H
