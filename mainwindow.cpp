#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>

#include "mainrenderwindow.h"

#include <iostream>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    RenderWindow = new MainRenderWindow(this);    
    RenderWindow->setWindowFlags(Qt::Window);
}

MainWindow::~MainWindow()
{
    releaseMouse();
    releaseKeyboard();
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    event->accept();
}

void MainWindow::show()
{
    QMainWindow::show();
    RenderWindow->show();
    RenderWindow->raise();
}


void MainWindow::on_actionShow_Render_Window_triggered()
{
    RenderWindow->show();
    RenderWindow->focusWidget();
    RenderWindow->raise();
}
