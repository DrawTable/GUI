#include "openfiledialog.h"
#include "filedialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QFrame>

OpenFileDialog::OpenFileDialog(QWidget *parent) : QDialog(parent) {

    /* layouts */

    QVBoxLayout* layout = new QVBoxLayout();
    QHBoxLayout* hlayout;

    /* title area */

    title = new QLabel("Open File");
    title->setStyleSheet("font: 24pt; color: white;");

    QFrame* line = new QFrame;
    line->setFrameStyle(QFrame::HLine | QFrame::Raised);
    line->setStyleSheet("background: white; border: 2px solid white;");

    layout->addWidget(title);
    layout->addWidget(line);

    /* files header */

    path = new QLabel(QDir::homePath());
    path->setStyleSheet("color: white; font: 14pt;");

    drive = new QPushButton("Drives");
    drive->setFixedSize(QSize(150, 50));
    drive->setStyleSheet("QPushButton{color: white; border: 0px; font: 14pt;} QPushButton::hover{border: 2px solid white;}");
    connect(drive, SIGNAL(clicked(bool)), this, SLOT(onDriveClicked()));

    hlayout = new QHBoxLayout();
    hlayout->addWidget(path);
    hlayout->addStretch();
    hlayout->addWidget(drive);

    layout->addLayout(hlayout);

    /* files area */

    model = new QFileSystemModel();
    model->setFilter(QDir::AllEntries | QDir::NoDot);

    view = new QListView();
    view->setModel(model);
    view->setRootIndex(model->setRootPath(path->text()));
    view->setStyleSheet("QListView{border: 2px solid white; } QListView::item{color: white; height: 50px;} QListView::item:hover{border: 2px solid white; } QListView::item:selected{border: 2px solid white;}");

    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(onViewClicked(QModelIndex)));

    layout->addWidget(view);

    /* open area */

    input = new Input();
    input->setFixedHeight(50);
    input->setStyleSheet("color: white; font: 14pt;");

    open = new QPushButton("Open");
    open->setFixedSize(QSize(150, 50));
    open->setStyleSheet("QPushButton{color: white; border: 0px; font: 14pt;} QPushButton::hover{border: 2px solid white;}");
    connect(open, SIGNAL(clicked(bool)), this, SLOT(onOpenClicked()));

    cancel = new QPushButton("Cancel");
    cancel->setFixedSize(QSize(150, 50));
    cancel->setStyleSheet("QPushButton{color: white; border: 0px; font: 14pt;} QPushButton::hover{border: 2px solid white;}");
    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(onCancelClicked()));

    hlayout = new QHBoxLayout();
    hlayout->addWidget(input);
    hlayout->addWidget(open);
    hlayout->addWidget(cancel);

    layout->addLayout(hlayout);

    /* keyboard */
    keyboard = new VirtualKeyboard(input, this);
    keyboard->setStyleSheet("font: 14pt;");
    keyboard->hide();
    connect(keyboard, SIGNAL(enterClicked()), this, SLOT(onOpenClicked()));

    layout->addWidget(keyboard);

    /* window */

    setLayout(layout);
    setStyleSheet("background: rgb(46,46,46);");
    showFullScreen();
}

void OpenFileDialog::onViewClicked(QModelIndex index) {
    path->setText(model->fileInfo(index).absoluteFilePath());
    if (model->fileInfo(index).isDir()) {
        view->setRootIndex(model->setRootPath(path->text()));
    } else if (model->fileInfo(index).isFile()) {
        input->setText(model->fileInfo(index).fileName());
    }
}

void OpenFileDialog::onOpenClicked() {
    SystemFileDialog::fileName = path->text();
    close();
}

void OpenFileDialog::onCancelClicked() {
    SystemFileDialog::fileName = "";
    close();
}

void OpenFileDialog::onDriveClicked() {
    path->setText("");
    view->setRootIndex(model->setRootPath(path->text()));
}
