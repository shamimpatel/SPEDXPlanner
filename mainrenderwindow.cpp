#include "mainrenderwindow.h"
#include "ui_mainrenderwindow.h"
#include <QKeyEvent>

MainRenderWindow::MainRenderWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainRenderWindow)
{
    ui->setupUi(this);
}

MainRenderWindow::~MainRenderWindow()
{
    releaseMouse();
    releaseKeyboard();
    delete ui;
}

void MainRenderWindow::keyPressEvent(QKeyEvent *event)
{
    event->accept();
}
