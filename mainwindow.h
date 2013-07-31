#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "crystal.h"
#include "spectrum.h"
#include "geometry.h"
#include <QStandardItemModel>

class MainRenderWindow;
class QLineEdit;
class QGraphicsScene;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *);
public slots:
    void show();
private slots:

    void on_actionShow_Render_Window_triggered();

    void on_LatticeConstantInput_textChanged(const QString &arg1);

    void on_pushButton_pressed();

    float CheckNumericalInput(QLineEdit* LineEdit, const QString &text, bool* bConverted);

    void on_MinEnergyInput_textChanged(const QString &arg1);

    void on_MaxEnergyInput_textChanged(const QString &arg1);

    void on_MinBraggAngleInput_textChanged(const QString &arg1);

    void on_MaxBraggAngleInput_textChanged(const QString &arg1);

    void on_actionExit_triggered();

    void on_SymmetrySelector_currentIndexChanged(int index);

public:
    void UpdateCrystallography();

private:
    Crystal TempCrystal;
    Spectrum BacklighterSpectrum;
    Geometry ExptGeometry;
    Ui::MainWindow *ui;
    MainRenderWindow *RenderWindow;
    QStandardItemModel *model;
};

#endif // MAINWINDOW_H
