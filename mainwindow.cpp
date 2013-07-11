#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include

#include <iostream>



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

void MainWindow::on_QuitButton_clicked()
{
    //std::cout << RenderWidget->x << std::endl;
}
