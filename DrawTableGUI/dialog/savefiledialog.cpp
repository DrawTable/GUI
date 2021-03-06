#include "savefiledialog.h"
#include "filedialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QFrame>
#include <QScrollBar>

SaveFileDialog::SaveFileDialog(QWidget *parent) : QDialog(parent)
{

    /* layouts */

    QVBoxLayout* layout = new QVBoxLayout();
    QHBoxLayout* hlayout;

    /* title area */

    title = new QLabel("Save File");
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
    view->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 45px; }");

    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(onViewClicked(QModelIndex)));

    layout->addWidget(view);

    /* save area */

    input = new Input();
    input->setFixedHeight(50);
    input->setStyleSheet("color: white; font: 14pt;");

    save = new QPushButton("Save");
    save->setFixedSize(QSize(150, 50));
    save->setStyleSheet("QPushButton{color: white; border: 0px; font: 14pt;} QPushButton::hover{border: 2px solid white;}");
    connect(save, SIGNAL(clicked(bool)), this, SLOT(onSaveClicked()));

    cancel = new QPushButton("Cancel");
    cancel->setFixedSize(QSize(150, 50));
    cancel->setStyleSheet("QPushButton{color: white; border: 0px; font: 14pt;} QPushButton::hover{border: 2px solid white;}");
    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(onCancelClicked()));

    hlayout = new QHBoxLayout();
    hlayout->addWidget(input);
    hlayout->addWidget(save);
    hlayout->addWidget(cancel);

    layout->addLayout(hlayout);

    /* keyboard */
    keyboard = new VirtualKeyboard(input, this);
    keyboard->setStyleSheet("font: 14pt;");
    keyboard->hide();
    connect(keyboard, SIGNAL(enterClicked()), this, SLOT(onSaveClicked()));

    layout->addWidget(keyboard);

    /* window */

    setLayout(layout);
    setStyleSheet("background: rgb(46,46,46);");
    showFullScreen();
}

void SaveFileDialog::onViewClicked(QModelIndex index)
{
    path->setText(model->fileInfo(index).absoluteFilePath());
    if (model->fileInfo(index).isDir())
    {
        view->setRootIndex(model->setRootPath(path->text()));
    }
    else if (model->fileInfo(index).isFile())
    {
        input->setText(model->fileInfo(index).fileName());
    }
}

void SaveFileDialog::onSaveClicked()
{
    SystemFileDialog::fileName = path->text() + QDir::separator() + input->text();
    close();
}

void SaveFileDialog::onCancelClicked()
{
    SystemFileDialog::fileName = "";
    close();
}

void SaveFileDialog::onDriveClicked()
{
    path->setText("");
    view->setRootIndex(model->setRootPath(path->text()));
}
