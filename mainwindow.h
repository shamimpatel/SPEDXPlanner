#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GLWidget;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_QuitButton_clicked();

private:
    Ui::MainWindow *ui;
    GLWidget *RenderWidget;
};

#endif // MAINWINDOW_H
