#include "errormanager.h"

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringBuilder>

ErrorManager* ErrorManager::instance = 0;

ErrorManager::ErrorManager(){
    nbTry = 0;
}

ErrorManager* ErrorManager::getInstance(){
    if (instance == nullptr){
        instance = new ErrorManager();
    }
    return instance;
}

void ErrorManager::onCalibrationError(int errorCode){
    Q_UNUSED(errorCode);

    QString textIntro = "An error occured during the calibration.\n\n\
1. Did you moved your camera?\n\
2. Can the camera see the screen entirely?\n\
3. Is your camera obstructed?\n\
4. Is your camera still accessible by your OS?\n\
5. Is your camera connected to your computer?\n\n\
Please restart to camera's selection step by using the 'restart' button.\n";

    QLabel* textLabel = new QLabel();
    textLabel->setText(textIntro);

    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->addStretch(1);
    textLayout->addWidget(textLabel, 0, Qt::AlignCenter);
    textLayout->addStretch(1);

    QPushButton* restart = new QPushButton();
    restart->setText("Restart");
    QObject::connect(restart, SIGNAL(clicked()), this, SLOT(selectCamera()));

    QPushButton* quit = new QPushButton();
    quit->setText("Quit");
    QObject::connect(quit, SIGNAL(clicked()), this, SLOT(quitApplication()));

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(restart);
    buttonsLayout->addWidget(quit);
    buttonsLayout->addStretch(1);

    QVBoxLayout* globalLayout = new QVBoxLayout();
    globalLayout->addLayout(textLayout);
    globalLayout->addStretch(1);
    globalLayout->addLayout(buttonsLayout);

    dialog = new QDialog();
    dialog->setWindowTitle("Calibration failure");
    dialog->setLayout(globalLayout);
    dialog->exec();
}

void ErrorManager::selectCamera(){
    emit cameraSelection();
    dialog->close();
}

void ErrorManager::quitApplication(){
     emit quitApp();
    dialog->close();
}


