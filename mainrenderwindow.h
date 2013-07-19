#ifndef MAINRENDERWINDOW_H
#define MAINRENDERWINDOW_H

#include <QDialog>

namespace Ui {
class MainRenderWindow;
}

class MainRenderWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit MainRenderWindow(QWidget *parent = 0);
    ~MainRenderWindow();
protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainRenderWindow *ui;
};

#endif // MAINRENDERWINDOW_H
