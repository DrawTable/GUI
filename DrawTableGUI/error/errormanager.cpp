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

    QString line1 = "An error occured during the calibration.\n\n";
    QString line2 = "Please check the list bellow :\n";
    QString line3 = "1. you moved your camera\n";
    QString line4 = "2. your camera is shutdown\n";
    QString line5 = "3. your camera is obstructed\n";
    QString line6 = "4. your camera is no longer connected to your computer\n\n";
    QString line7 = "If it's the 1st point, please restart to camera's selection step by using the 'select' button.\n";
    QString line8 = "If it's one of the other points, fix it and restart the calibration by using the 'restart' button.";
    QString textIntro = line1 % line2 % line3 % line4 % line5 %line6 % line7 % line8;

    QLabel* textLabel = new QLabel();
    textLabel->setText(textIntro);

    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->addStretch(1);
    textLayout->addWidget(textLabel, 0, Qt::AlignCenter);
    textLayout->addStretch(1);

    QPushButton* select = new QPushButton();
    select->setText("Select");
    QObject::connect(select, SIGNAL(clicked()), this, SLOT(selectCamera()));

    QPushButton* quit = new QPushButton();
    quit->setText("Quit");
    QObject::connect(quit, SIGNAL(clicked()), this, SLOT(quitApplication()));

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(select);
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


