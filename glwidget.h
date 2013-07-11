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
    //int x;
signals:
    
public slots:



protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    bool bMouseGrabbed;
    QPoint PreviousMousePos;
    QMatrix4x4 pMatrix;
    QGLShaderProgram shaderProgram;
    QVector<QVector4D> vertices;
    QVector<QVector3D> VertexColours;

    QVector3D CameraPosition;
    QVector3D CameraDirection;
    QVector3D CameraUp;
    QVector3D CameraTarget;
private:
    //void UpdateCameraAngles(double pitch, double yaw, double roll);
    void UpdateCamera(double delta_theta, double delta_phi);
    void MoveCamera(QVector3D Delta);
    void MoveCamera(bool forward, bool back, bool left, bool right);

    double theta,phi;
    double alpha,beta,distance;
};

#endif // GLWIDGET_H
