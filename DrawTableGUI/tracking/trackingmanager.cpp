#include "trackingmanager.h"
#include "LedDetection/leddetector.h"
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
    qDebug() << "TM::onStratCalibration";

    Mat frame;

    // Lancement de la capture avec la webcam
    cap = new VideoCapture(cameraId);

    // Lecture d'une image
    if (!cap->read(frame)) {
        // Erreur: la lecture de la frame a échoué
        cap->release();
        emit calibrationError(1);
        cerr << "la lecture de la frame a échoué" << endl;
        emit finished();
        return;
    }

    //namedWindow("frame", WINDOW_KEEPRATIO);
    //imshow("frame", frame);

    ScreenDetector sd(frame, width, height);

    ScreenDetector::Error err;

    // Récupération de la matrice de transformation
    transformMatrix = sd.getTransformationMatrix(err);

    // always check error before using the transformatrix
    if(err.hasError()){
        cerr << err.getErrorTitle() << ":\n" << err.getErrorMessage() << endl;
        cap->release();
        emit calibrationError(1);
        emit finished();
        return;
    }

   emit calibrationSuccess();
    mainLoop();
}

void TrackingManager::onStartStylusCalibration()
{
    qDebug() << "Start Calibration" << endl;

    for(int i=0; i < 100; i++){
        usleep(500);
        emit stylusCalibrationProgress(i);
    }

    qDebug() << "Calibration Finished" << endl;

    bool success = true;
    if(success)
        emit stylusCalibrationSuccess();
    else
        emit stylusCalibrationError(1);

}

// Boucle principal du thread
void TrackingManager::mainLoop() {

    Controller ctrl;
    ctrl.start();
    bool click = false;
    bool release = false;
    LedDetector* ledDetector = LedDetector::getInstance();
    Point stylusPoint;
    forever {
        Mat frame;

        if(!cap->read(frame)) {
            // TODO send error to the Main Window (frame cannot be read from the webcam)
            cerr << "frame cannot be read from the webcam" << endl;
            return;
        }

        // Tracking de la led
       ledDetector->setImage(frame);
       stylusPoint = ledDetector->debugLedDetection();

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
