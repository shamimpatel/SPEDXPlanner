#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <QMatrix>
#include <QGLShaderProgram>
#include <cmath>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();
    bool HasGrabbedInput();

    QPoint MapFromScreenspaceToLocal(double x, double y);

signals:
    
public slots:

private slots:
    void InputTimer();


protected:
    void initializeGL();
    void resizeGL(int w, int h);
    //void paintGL();

    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private:
    bool bInputGrabbed;
    QPoint PreviousMousePos;
    QMatrix4x4 ProjectionMatrix;
    QMatrix4x4 ViewMatrix;
    QGLShaderProgram shaderProgram;
    QVector<QVector4D> vertices;
    QVector<QVector3D> VertexColours;

    QVector3D CameraPosition;
    QVector3D CameraDirection;
    QVector3D CameraUp;
    QVector3D CameraTarget;


    enum MovementKey
    {
        Forward,
        Backward,
        Left,
        Right
    };


    double theta,phi,roll;
    bool bForwardKey,bBackwardKey,bLeftKey,bRightKey;
    int msPerFrame;
    QTimer *InputTickTimer;
private:
    void UpdateCamera(double delta_theta, double delta_phi, double delta_roll);
    void MoveCamera(QVector3D Delta);
    void SetMovementKey( GLWidget::MovementKey Key, bool bStatus);

    void GrabInput( bool bGrab );
    void PerformCameraMovement();
};

#endif // GLWIDGET_H
