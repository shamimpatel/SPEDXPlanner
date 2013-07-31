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

    /*scene = new QGraphicsScene;
    ui->ChartArea->setScene(scene);*/
    model=new QStandardItemModel(0,2,this);

    RenderWindow = new MainRenderWindow(this);
    RenderWindow->setWindowFlags(Qt::Window);

    ui->LatticeConstantInput->setText("3.31");
    TempCrystal.SetLatticeConstant(3.31f);
    TempCrystal.SetSymmetry(Crystal::Symmetry::CUBIC_BCC);

    ui->SymmetrySelector->addItem("BCC", int(Crystal::Symmetry::CUBIC_BCC));
    ui->SymmetrySelector->addItem("FCC", int(Crystal::Symmetry::CUBIC_FCC));

    BacklighterSpectrum.SetMinE( 3.0f );
    BacklighterSpectrum.SetMaxE( 9.0f );

    ui->tabWidget->setCurrentIndex(1);

    ui->chart->setModel(model);
    ui->chart->setting().border().setRight(10);
    ui->chart->setting().border().setLeft(40);
    ui->chart->setting().border().setBottom(40);
    ui->chart->setting().border().setBackgroundColor(QColor(255,255,255));

    ui->chart->setting().grid().horizontalTick().setTickMajor(10);
    ui->chart->setting().grid().horizontalTick().setTickMinor(4);
    ui->chart->setting().grid().verticalTick().setTickMajor(5);
    ui->chart->setting().grid().verticalTick().setTickMinor(4);
    ui->chart->setting().grid().setBackgroundColor(QColor(255,255,255));
    ui->chart->setting().grid().setForegroundColor(QColor(0,0,0));
    //ui->chart->setting().scale().
    //ui->chart->setting().scale().setAutoCurrentLimit(true);
    //ui->chart->setting().scale().setNominalAutoLimit(true);
    ui->chart->updateChart();
    UpdateCrystallography();
}

MainWindow::~MainWindow()
{
    releaseMouse();
    releaseKeyboard();
    delete ui;
    delete model;
    //delete scene;
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

void MainWindow::resizeEvent(QResizeEvent *)
{

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
    if(bConvertedProperly && a0 >= 0)
    {
        TempCrystal.SetLatticeConstant(a0);
        UpdateCrystallography();
    }
}

void MainWindow::on_pushButton_pressed()
{
    UpdateCrystallography();
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
    if(bConvertedProperly && MinE >= 0)
    {
        BacklighterSpectrum.SetMinE(MinE);
        UpdateCrystallography();
    }
}

void MainWindow::on_MaxEnergyInput_textChanged(const QString &arg1)
{
    bool bConvertedProperly;
    float MaxE = CheckNumericalInput(ui->MaxEnergyInput,arg1,&bConvertedProperly);
    if(bConvertedProperly && MaxE > 0)
    {
        BacklighterSpectrum.SetMaxE(MaxE);
        UpdateCrystallography();
    }
}

void MainWindow::on_MinBraggAngleInput_textChanged(const QString &arg1)
{
    bool bConvertedProperly;
    float Angle = CheckNumericalInput(ui->MinBraggAngleInput,arg1,&bConvertedProperly);
    if(bConvertedProperly && Angle >= 0)
    {
        ExptGeometry.OverrideMinBraggAngle(Angle);
        UpdateCrystallography();
    }
}

void MainWindow::on_MaxBraggAngleInput_textChanged(const QString &arg1)
{
    bool bConvertedProperly;
    float Angle = CheckNumericalInput(ui->MaxBraggAngleInput,arg1,&bConvertedProperly);
    if(bConvertedProperly && Angle >= 0)
    {
        ExptGeometry.OverrideMaxBraggAngle(Angle);
        UpdateCrystallography();
    }
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_SymmetrySelector_currentIndexChanged(int index)
{
    bool bOk;
    QVariant Data = ui->SymmetrySelector->itemData(index);
    Crystal::Symmetry Symmetry = (Crystal::Symmetry)Data.toInt(&bOk);
    if(!bOk)
    {
        cout << "Could not convert properly." << endl;
        return;
    }
    TempCrystal.SetSymmetry(Symmetry);
    UpdateCrystallography();
}

void MainWindow::UpdateCrystallography()
{
    model->clear();

    Crystallography C;
    C.SetTarget(&TempCrystal);
    C.SetSpectrum(&BacklighterSpectrum);
    C.SetGeometry(&ExptGeometry);
    C.CalculateDiffractionSpectrum(model, ui->TempOutput);

    int MinE = floor(BacklighterSpectrum.GetMinE() + 0.5);
    int MaxE = floor(BacklighterSpectrum.GetMaxE() + 0.5);

    int NumTicks = 1.0*(MaxE - MinE) + 1;
    ui->chart->setting().grid().horizontalTick().setTickMajor(NumTicks);
    ui->chart->setting().scale().setCurrentLimit(ChartXYLimit(ChartXYLimitAxis(float(MinE),float(MaxE)),ChartXYLimitAxis(0,50)));
    ui->chart->updateChart();
}
