#include "trackingmanager.h"

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
    Mat frame;

    // get an image
    // frame = imread("/home/sacha/Projets/DrawTable/Test_calibration/Test_calibration/IMG_1878.JPG");

    // Lancement de la capture avec la webcam
    cap = new VideoCapture(0);

    // Lecture d'une image
    if (!cap->read(frame)) {
        // Erreur: la lecture de la frame a échoué
        emit calibrationError(1);
        cerr << "la lecture de la frame a échoué" << endl;
        return;
    }

    ScreenDetector sd(frame);

    ScreenDetector::Error err;

    // Récupération de la matrice de transformation
    transformMatrix = sd.getTransformationMatrix(err);

    // always check error before using the transformatrix
    if(err.hasError()){
        cerr << err.getErrorTitle() << ":\n" << err.getErrorMessage() << endl;
        emit calibrationError(2);
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
        return;
    }
}

// Boucle principal du thread
void TrackingManager::mainLoop() {
    forever {
        Mat frame;

        if(!cap->read(frame)) {
            // TODO send error to the Main Window (frame cannot be read from the webcam)
            cerr << "frame cannot be read from the webcam" << endl;
            return;
        }

        // TODO get the stylus position (x; y) from the `frame` Mat.
        int x = 0;
        int y = 0;
        Point stylusPoint(x, y);
        Point screenPoint = ScreenDetector::transformPoint(stylusPoint, transformMatrix);

        // Envoie les coordonnées du stylet afin de bouger la souris
        emit mouseMove(screenPoint.x, screenPoint.y);
    }
}
