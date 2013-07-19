#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainRenderWindow;


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

public slots:
    void show();
private slots:

    void on_actionShow_Render_Window_triggered();

private:
    Ui::MainWindow *ui;
    MainRenderWindow *RenderWindow;
};

#endif // MAINWINDOW_H
