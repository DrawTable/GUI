#ifndef ERRORMANAGER
#define ERRORMANAGER

#include <QObject>
#include <QDialog>

/**
 * @brief Error Handler
 *
 * The purpose of this class is to show a pop-up when there is an error with the calibration process.
 * Some information of why the calibration may have fail is shown to the user and then,
 * the user has to choice to restart the calibration or to quit the program.
 *
 * This class is a Singleton.
 */
class ErrorManager : public QObject
{
    Q_OBJECT

public:
    static ErrorManager* getInstance();

public slots:
    // slots internes
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
    void cameraSelection();
    void quitApp();
};


#endif // ERRORMANAGER
