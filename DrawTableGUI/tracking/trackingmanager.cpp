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
