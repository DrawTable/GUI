#ifndef ERRORMANAGER
#define ERRORMANAGER

#include <QObject>
#include <QDialog>

#include <QDebug>

class ErrorManager : public QObject {
    Q_OBJECT

public:
    static ErrorManager* getInstance();
public slots:
    // slots internes
    void restartCalibration();
    void selectCamera();
    void quitApplication();

    // communication tracking manager <-> error manager
    void onCalibrationError(int errorCode);

private:
    ErrorManager();

    static ErrorManager* instance;
    QDialog* dialog;
    const int NB_TRY_MAX = 3;
    int nbTry;

signals:
    void startCalibration();
    void cameraSelection();
    void quitApp();
};


#endif // ERRORMANAGER

