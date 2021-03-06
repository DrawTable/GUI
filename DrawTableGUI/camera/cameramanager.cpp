#include "cameramanager.h"

CameraManager* CameraManager::instance = 0;

CameraManager::CameraManager()
{
}

CameraManager* CameraManager::getInstance()
{
    if (instance == nullptr)
    {
        instance = new CameraManager();
    }

    return instance;
}

void CameraManager::initCameras()
{
    // Tentative de détection de 10 caméras
    for (int i = 0; i < 10; i++)
    {
        cv::VideoCapture* cap = new cv::VideoCapture(i);
        if (cap->isOpened())
        {
            qDebug() << "Camera #" << i << " is working.";
            captures.push_back(cap);
        }
    }
}

int CameraManager::countCameras()
{
    return captures.size();
}

void CameraManager::listCameras()
{
    // Création des vues pour les caméras
    for (unsigned int i = 0; i < captures.size(); i++)
    {
        cv::VideoCapture* cap = captures.at(i);
        cv::Mat frame;
        cap->read(frame);

        std::string windowName = "Camera #" + std::to_string(i);
        cv::namedWindow(windowName, cv::WINDOW_KEEPRATIO);

        cv::setMouseCallback(windowName, mouseHandler, (void *) (intptr_t) i);
    }

    startUpdater();
}

// Gestion d'événements sur les fenêtres d'aperçu
void CameraManager::mouseHandler(int event, int x, int y, int flags, void * params)
{
    Q_UNUSED(x)
    Q_UNUSED(y)
    Q_UNUSED(flags)

    // On ne gère que le clique gauche
    if(event != cv::EVENT_LBUTTONDOWN)
    {
        return;
    }

    int i = (intptr_t) params;

    qDebug() << "Clicked: " << i;

    CameraManager::getInstance()->destroyWindows();
    CameraManager::getInstance()->emitCameraId(i);
}

void CameraManager::emitCameraId(int cameraId)
{
    emit cameraChoosen(cameraId);
}

void CameraManager::destroyWindows()
{
    updater->stopUpdater();

    QThread::msleep(500);
    onUpdaterFinished();
}

void CameraManager::onUpdaterFinished()
{
    for (unsigned int i = 0; i < captures.size(); i++)
    {
        std::string windowName = "Camera #" + std::to_string(i);
        cv::destroyWindow(windowName);
        cv::VideoCapture* cap = captures.at(i);
        cap->release();
    }

    captures.clear();
}

void CameraManager::startUpdater()
{
    thread = new QThread;
    updater = new CameraManagerUpdater(captures);
    updater->moveToThread(thread);

    // Lancement et arrêt du thread
    connect(thread, SIGNAL(started()), updater, SLOT(process()));
    connect(updater, SIGNAL(finished()), thread, SLOT(quit()));
    connect(updater, SIGNAL(finished()), updater, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
