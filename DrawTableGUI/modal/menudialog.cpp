#include "menudialog.h"

MenuDialog::MenuDialog(MainWindow* parent) : QDialog(parent) {
    _new = new QPushButton(this);
    _new->setFixedSize(QSize(128, 128));
    _new->setAutoFillBackground(true);
    _new->setStyleSheet("QPushButton{ border-image: url(:/menu/icons/new_off.png) 0 0 0 0 stretch stretch; } QPushButton:hover{ border-image: url(:/menu/icons/new_on.png) 0 0 0 0 stretch stretch; }");
    connect(_new, SIGNAL(clicked(bool)), this, SLOT(onNewClicked()));

    open = new QPushButton(this);
    open->setFixedSize(QSize(128, 128));
    open->setStyleSheet("QPushButton{ border-image: url(:/menu/icons/open_off.png) 0 0 0 0 stretch stretch; } QPushButton:hover{ border-image: url(:/menu/icons/open_on.png) 0 0 0 0 stretch stretch; }");
    connect(open, SIGNAL(clicked(bool)), this, SLOT(onOpenClicked()));

    save = new QPushButton(this);
    save->setFixedSize(QSize(128, 128));
    save->setStyleSheet("QPushButton{ border-image: url(:/menu/icons/save_off.png) 0 0 0 0 stretch stretch; } QPushButton:hover{ border-image: url(:/menu/icons/save_on.png) 0 0 0 0 stretch stretch; }");
    connect(save, SIGNAL(clicked(bool)), this, SLOT(onSaveClicked()));

    print = new QPushButton(this);
    print->setFixedSize(QSize(128, 128));
    print->setStyleSheet("QPushButton{ border-image: url(:/menu/icons/print_off.png) 0 0 0 0 stretch stretch; } QPushButton:hover{ border-image: url(:/menu/icons/print_on.png) 0 0 0 0 stretch stretch; }");
    connect(print, SIGNAL(clicked(bool)), this, SLOT(onPrintClicked()));

    quit = new QPushButton(this);
    quit->setFixedSize(QSize(128, 128));
    quit->setStyleSheet("QPushButton{ border-image: url(:/menu/icons/quit_off.png) 0 0 0 0 stretch stretch; } QPushButton:hover{ border-image: url(:/menu/icons/quit_on.png) 0 0 0 0 stretch stretch; }");
    connect(quit, SIGNAL(clicked(bool)), this, SLOT(onQuitClicked()));

    layout = new QHBoxLayout();
    layout->addWidget(_new);
    layout->addWidget(open);
    layout->addWidget(save);
    layout->addWidget(print);
    layout->addWidget(quit);

    setLayout(layout);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet("background: rgb(46,46,46);");
    setWindowOpacity(0.8);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
}

MenuDialog::~MenuDialog() {

}

void MenuDialog::onNewClicked() {
    MainWindow* window = dynamic_cast<MainWindow*>(parent());
    window->onNewTriggered();
    close();
}

void MenuDialog::onOpenClicked() {
    MainWindow* window = dynamic_cast<MainWindow*>(parent());
    window->onOpenTriggered();
    close();
}

void MenuDialog::onSaveClicked() {
    MainWindow* window = dynamic_cast<MainWindow*>(parent());
    window->onSaveTriggered();
    close();
}

void MenuDialog::onPrintClicked() {
    MainWindow* window = dynamic_cast<MainWindow*>(parent());
    window->onPrintTriggered();
    close();
}

void MenuDialog::onQuitClicked() {
    MainWindow* window = dynamic_cast<MainWindow*>(parent());
    window->onQuitTriggered();
    close();
}
