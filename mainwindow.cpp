#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>

#include <iostream>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    RenderWidget = ui->RenderWidget;
}

MainWindow::~MainWindow()
{
    releaseMouse();
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    event->accept();
}
