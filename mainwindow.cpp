#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>

#include "mainrenderwindow.h"
#include "crystallography.h"

#include <iostream>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    RenderWindow = new MainRenderWindow(this);    
    RenderWindow->setWindowFlags(Qt::Window);

    ui->LatticeConstantInput->setText("3.31");
    TempCrystal.SetLatticeConstant(3.31f);

    BacklighterSpectrum.MinE = 3.0f;
    BacklighterSpectrum.MaxE = 9.0f;
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
    //RenderWindow->show();
    //RenderWindow->raise();
}


void MainWindow::on_actionShow_Render_Window_triggered()
{
    RenderWindow->show();
    RenderWindow->focusWidget();
    RenderWindow->raise();
}

void MainWindow::on_LatticeConstantInput_textChanged(const QString &arg1)
{     
    bool bConvertedProperly;
    float a0 = CheckNumericalInput(ui->LatticeConstantInput,arg1,&bConvertedProperly);
    if(bConvertedProperly)
    {
        TempCrystal.SetLatticeConstant(a0);
    }
}

void MainWindow::on_pushButton_pressed()
{
    Crystallography C;
    C.SetTarget(&TempCrystal);
    C.SetSpectrum(&BacklighterSpectrum);
    C.SetGeometry(&ExptGeometry);
    C.CalculateDiffractionSpectrum(ui->TempOutput);
}

float MainWindow::CheckNumericalInput(QLineEdit *LineEdit, const QString &text, bool *bConverted)
{
    float a0 = text.toFloat(bConverted);
    if(*bConverted)
    {
        LineEdit->setStyleSheet("background:white");
    }
    else
    {
        LineEdit->setStyleSheet("background:red");
    }
    return a0;
}


void MainWindow::on_MinEnergyInput_textChanged(const QString &arg1)
{
    bool bConvertedProperly;
    float MinE = CheckNumericalInput(ui->MinEnergyInput,arg1,&bConvertedProperly);
    if(bConvertedProperly)
    {
        BacklighterSpectrum.MinE = MinE;
    }
}

void MainWindow::on_MaxEnergyInput_textChanged(const QString &arg1)
{
    bool bConvertedProperly;
    float MaxE = CheckNumericalInput(ui->MaxEnergyInput,arg1,&bConvertedProperly);
    if(bConvertedProperly)
    {
        BacklighterSpectrum.MaxE = MaxE;
    }
}

void MainWindow::on_MinBraggAngleInput_textChanged(const QString &arg1)
{
    bool bConvertedProperly;
    float Angle = CheckNumericalInput(ui->MinBraggAngleInput,arg1,&bConvertedProperly);
    if(bConvertedProperly)
    {
        ExptGeometry.OverrideMinBraggAngle(Angle);
    }
}

void MainWindow::on_MaxBraggAngleInput_textChanged(const QString &arg1)
{
    bool bConvertedProperly;
    float Angle = CheckNumericalInput(ui->MaxBraggAngleInput,arg1,&bConvertedProperly);
    if(bConvertedProperly)
    {
        ExptGeometry.OverrideMaxBraggAngle(Angle);
    }
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}
