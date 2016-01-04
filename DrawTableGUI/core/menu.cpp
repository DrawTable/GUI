#include "menu.h"

Menu::Menu(MainWindow* parent) : QDialog(parent)
{
    _new = new Menu::MenuItem(":/menu/icons/new_on.png", ":/menu/icons/new_off.png", this);
    connect(_new, SIGNAL(clicked(bool)), this, SLOT(onNewClicked()));

    open = new Menu::MenuItem(":/menu/icons/open_on.png", ":/menu/icons/open_off.png", this);
    connect(open, SIGNAL(clicked(bool)), this, SLOT(onOpenClicked()));

    save = new Menu::MenuItem(":/menu/icons/save_on.png", ":/menu/icons/save_off.png", this);
    connect(save, SIGNAL(clicked(bool)), this, SLOT(onSaveClicked()));

    print = new Menu::MenuItem(":/menu/icons/print_on.png", ":/menu/icons/print_off.png", this);
    connect(print, SIGNAL(clicked(bool)), this, SLOT(onPrintClicked()));

    quit = new Menu::MenuItem(":/menu/icons/quit_on.png", ":/menu/icons/quit_off.png", this);
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

Menu::~Menu()
{

}

void Menu::onNewClicked()
{
    MainWindow* window = dynamic_cast<MainWindow*>(parent());
    window->onNewTriggered();
    close();
}

void Menu::onOpenClicked()
{
    MainWindow* window = dynamic_cast<MainWindow*>(parent());
    window->onOpenTriggered();
    close();
}

void Menu::onSaveClicked()
{
    MainWindow* window = dynamic_cast<MainWindow*>(parent());
    window->onSaveTriggered();
    close();
}

void Menu::onPrintClicked()
{
    MainWindow* window = dynamic_cast<MainWindow*>(parent());
    window->onPrintTriggered();
    close();
}

void Menu::onQuitClicked()
{
    MainWindow* window = dynamic_cast<MainWindow*>(parent());
    window->onQuitTriggered();
    close();
}

Menu::MenuItem::MenuItem(QString pictureOn, QString pictureOff, QWidget *parent) : QPushButton(parent)
{
    setFixedSize(QSize(128, 128));
    setStyleSheet("QPushButton{ border-image: url(" + pictureOff + ") 0 0 0 0 stretch stretch; } QPushButton:hover{ border-image: url(" + pictureOn + ") 0 0 0 0 stretch stretch; }");
}
