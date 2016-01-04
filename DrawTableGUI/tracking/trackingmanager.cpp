#include "trackingmanager.h"
#include "tracking/ledtracking/ledtracker.h"
#include "controller.h"

TrackingManager::TrackingManager(int cameraId, QObject *parent) : QObject(parent)
{
    this->cameraId = cameraId;
}

void TrackingManager::process() {
    emit showGreenScreen();
}

// Lance le processus de calibration
void TrackingManager::onStratCalibration(int width, int height) {
    Mat frame;

    // Lancement de la capture avec la webcam
    cap = new VideoCapture(cameraId);

    // Lecture d'une image
    if (!cap->read(frame)) {
        // Erreur: la lecture de la frame a échoué
        cap->release();
        emit calibrationError(1);
        cout << "la lecture de la frame a échoué" << endl;
        qDebug() << "TM::onStratCalibration";
        emit finished();
        return;
    }

    ScreenDetector sd(frame, width, height);
    ScreenDetector::Error err;

    // Récupération de la matrice de transformation
    transformMatrix = sd.getTransformationMatrix(err);

    // always check error before using the transformatrix
    if(err.hasError()){
        cout << err.getErrorTitle() << ":\n" << err.getErrorMessage() << endl;
        cap->release();
        emit calibrationError(1);
        emit finished();
        return;
    }

   emit calibrationSuccess();
}

void TrackingManager::onStartStylusCalibration()
{
    qDebug() << "[TrackingManager] Start Calibration" << endl;

    LedTracker* ledTracker = LedTracker::getInstance();
    ledTracker->activateDebugAnalysis();
    ledTracker->setObjectType(LedTracker::STYLUS);
    ledTracker->setMode(LedTracker::CALIBRATION);

    if(!cap->isOpened()){
        cerr << "Cannot open the camera" << endl;
        return;
    }

    Mat frame;
    while(!ledTracker->isModelReady()){

        if(!cap->read(frame)){
            // TODO send error to the Main Window (frame cannot be read from the webcam)
           cerr << "frame cannot be read from the webcam" << endl;
           return;
        }

        ledTracker->setFrame(frame);
        ledTracker->calibrate();

        emit stylusCalibrationProgress(ledTracker->getCurrentCalibrationStep());
    }

    qDebug() << "calibration finished";
    mainLoop();
}

// Boucle principal du thread
void TrackingManager::mainLoop() {

    Controller ctrl;
    ctrl.start();
    bool click = false;
    bool release = false;
    LedTracker* ledTracker = LedTracker::getInstance();

    ledTracker->setMode(LedTracker::TRACKING);

    qDebug() << "Start tracking" << endl;
    Point stylusPoint;
    forever {
        Mat frame;

        if(!cap->read(frame)) {
            // TODO send error to the Main Window (frame cannot be read from the webcam)
            cerr << "frame cannot be read from the webcam" << endl;
            return;
        }

        // Tracking de la led
       ledTracker->setFrame(frame);
       stylusPoint = ledTracker->findObjectPosition(LedTracker::FAST);

        cout << stylusPoint << endl;

       // Envoie les coordonnées du stylet afin de bouger la souris
       if(stylusPoint.x > 0 && stylusPoint.y > 0 && stylusPoint.x < 1440 && stylusPoint.y < 800){

           stylusPoint = ScreenDetector::transformPoint(stylusPoint, transformMatrix);
           cout << stylusPoint << endl;
           ctrl.mouseMove(stylusPoint.x, stylusPoint.y);
           if(!click){
               ctrl.mousePressed();
               release = false;
           }

       } else {
           click = false;
           if(!release){
               ctrl.mouseReleased();
               release = true;
           }
       }

    }
}
