#include "trackingmanager.h"
#include <QDebug>

TrackingManager::TrackingManager(QObject *parent) : QObject(parent)
{

}

/*
1. Attendre que la GUI soit prête pour la calibration
2. Lancer la callibration (simule avec sleep)
2.a Si la calibration et ok envoit un signal calibrationSucess
2.b Sil la calibration n’a pas marché, envoi un signal calibrationError avec la raison et recommence la calibration
3. Commencer le tracking (simule en lançant la webcam et en lisant les frame, jusqu’a ce que l’on arrête le programme)
4. Avant de quitter le programme. demande une interuption du thread pour tout libérer (SIGNAL : QApplication::aboutToQuit())
*/
void TrackingManager::process() {
    emit showGreenScreen();
}

// Lance le processus de calibration
void TrackingManager::onStratCalibration() {
    // TODO Détéction de l'écran vert avec OpenCV, matrice de transformation, etc.
    // get an image
    Mat img = imread("/home/sacha/Projets/DrawTable/Test_calibration/Test_calibration/IMG_1878.JPG");
    ScreenDetector sd(img);

    // Start webcam
    // cap = new VideoCapture(0);
    // webCam = true;

    ScreenDetector::Error err;

    // Récupération de la matrice de transformation
    Mat transformMatrix = sd.getTransformationMatrix(err);

    // always check error before using the transformatrix
    if(err.hasError()){
        cerr << err.getErrorTitle() << ":\n" << err.getErrorMessage() << endl;
        emit calibrationError(1);
        return;
    }

    // test if we can transform a point
    Point p1(1756, 1010);
    Point p2 = ScreenDetector::transformPoint(p1, transformMatrix);
    cout << p1 << " -> " << p2 << endl;

    // test the result of a point outside of the screen
    p1 = Point(0, 0);
    p2 = ScreenDetector::transformPoint(p1, transformMatrix);
    cout << p1 << " -> " << p2 << endl; // will be p1 and p2 are < 0

    bool success = true;
    if (success) {
        emit calibrationSuccess();
    } else {
        int errorCode = -1;
        emit calibrationError(errorCode);
    }

    // Commencer le tracking (simule en lançant la webcam et en lisant les frame, jusqu’a ce que l’on arrête le programme)

    // TODO envoyer les coordonnées avec un signal-slot
}

void TrackingManager::mainLoop() {
    forever {
        // Envoie les coordonnées du stylet afin de bouger la souris
        emit mouseMove(0, 0);
    }
}
