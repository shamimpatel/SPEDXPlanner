#include "glwidget.h"
#include <QtGui/QMouseEvent>
#include <QTimer>
#include <iostream>
using namespace std;

#include "VectorHelpers.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    setMouseTracking(true);
    bInputGrabbed = false;
    PreviousMousePos = QCursor::pos();
    alpha = 0;
    beta = 0;
    distance = 2.5;

    theta = 180;
    phi =   0;

    CameraPosition = QVector3D(0,0,2.5);
    //CameraDirection = -1*CameraPosition.normalized(); //point towards origin
    //CameraTarget = CameraPosition + 1.0*CameraDirection;
    //CameraUp = QVector3D(0,-1,0);
    UpdateCamera(0.0,0.0);
}

bool GLWidget::HasGrabbedInput()
{
    return bInputGrabbed;
}

void GLWidget::TestTimer()
{
    /*cout << "Timer gone!" << endl;
    QTimer *Timer;
    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(TestTimer()));
    Timer->start(1000);*/
}


void GLWidget::initializeGL() {

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);
    qglClearColor(QColor(Qt::black));
    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, ":/vertexShader.vsh");
    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, ":/fragmentShader.fsh");
    shaderProgram.link();
    vertices << QVector4D(1, 0, -2, 1) << QVector4D(0, 1, -2, 1) << QVector4D(-3, 0, -2, 1);
    vertices << QVector4D(1, 2, 0, 1) << QVector4D(0, 2, 1, 1) << QVector4D(-1, 2, 0, 1);

    VertexColours << QVector3D(1,0,0) << QVector3D(1,1,0) << QVector3D(0,1,0);
    VertexColours << QVector3D(0,1,0) << QVector3D(0,1,1) << QVector3D(0,0,1);
}

void GLWidget::resizeGL(int width, int height) {
    /*glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho2D(0, w, 0, h); // set origin to bottom left corner
    glOrtho(0, w, h, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/

    if (height == 0) {
        height = 1;
    }
    pMatrix.setToIdentity();
    pMatrix.perspective(60.0, (float) width / (float) height, 0.001f, 1000.0f);
    glViewport(0, 0, width, height);

}

/*
void GLWidget::UpdateCameraAngles(double pitch, double yaw, double roll)
{

    QMatrix4x4 rotMatrix;

    //perform pitch
    rotMatrix.setToIdentity();
    QVector3D PitchRotAxis = QVector3D::crossProduct(CameraUp,CameraDirection);
    PitchRotAxis.normalize();
    rotMatrix.rotate(pitch,PitchRotAxis);
    CameraDirection = rotMatrix * CameraDirection;
    CameraUp = rotMatrix * CameraUp;

    CameraUp.normalize();
    CameraDirection.normalize();

    //perform yaw
    rotMatrix.setToIdentity();
    rotMatrix.rotate(yaw,CameraUp);
    CameraDirection = rotMatrix*CameraDirection;
    CameraDirection.normalize();


    //ignore roll for now
    roll;

}
*/

QMatrix4x4 MakeMatrixRotationYawPitchRoll( float roll, float pitch, float yaw )
{
    QMatrix4x4 out;
    out.setToIdentity();
    out.rotate(yaw,0,1,0);
    out.rotate(roll,1,0,0);
    out.rotate(pitch,0,0,1);
    return out;
}



void GLWidget::UpdateCamera(double delta_theta, double delta_phi)
{
    theta += delta_theta;
    phi += delta_phi;

    if(theta > 360) theta -= 360;
    if(theta < 0)   theta += 360;

    if(phi > 360) phi -= 360;
    if(phi < 0)   phi += 360;

    /*CameraDirection = CreateSphericalVector(1.0,Deg2Rad(theta),Deg2Rad(phi));
    QMatrix4x4 rotMatrix;
    rotMatrix.setToIdentity();
    rotMatrix.rotate(theta,0,1,0);
    rotMatrix.rotate(phi,0,0,1);

    //CameraUp = rotMatrix * QVector3D(0,-1,0);
    CameraUp = QVector3D(0,0,1);*/


}

void GLWidget::MoveCamera(QVector3D Delta)
{
    CameraPosition += Delta;
}

void GLWidget::MoveCamera(bool forward, bool back, bool left, bool right)
{
    QVector3D Delta(0,0,0);
    if( forward )
    {
        Delta += 0.2f*CameraDirection;
    }
    if( back )
    {
        Delta -= 0.2f*CameraDirection;
    }

    QVector3D vLeft = QVector3D::crossProduct(CameraUp,CameraDirection);
    if( left )
    {
        Delta += 0.2f*vLeft;
    }
    if( right )
    {
        Delta -= 0.2f*vLeft;
    }

    MoveCamera(Delta);
}


void GLWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 mMatrix;
    mMatrix.translate(QVector3D(0,0,0));


    QMatrix4x4 vMatrix;

    QMatrix4x4 rotMatrix = MakeMatrixRotationYawPitchRoll(theta,0.0,phi);
    CameraDirection = rotMatrix * QVector3D(0,0,1);
    CameraUp    = rotMatrix * QVector3D(0,-1,0);
    CameraTarget = CameraPosition + 1.0*CameraDirection;

    vMatrix.lookAt(CameraPosition,CameraTarget,CameraUp);

    /*cout << "Pos: "; PrintVector(CameraPosition);
    cout << "Target: "; PrintVector(CameraTarget);
    cout << "Direction: "; PrintVector(CameraDirection);
    cout << "Up: "; PrintVector(CameraUp);*/

    shaderProgram.bind();
    shaderProgram.setUniformValue("mvpMatrix", pMatrix * vMatrix * mMatrix);
    //shaderProgram.setUniformValue("colour", QColor(Qt::white));

    shaderProgram.setAttributeArray("colour", VertexColours.constData());
    shaderProgram.enableAttributeArray("colour");

    shaderProgram.setAttributeArray("vertex", vertices.constData());
    shaderProgram.enableAttributeArray("vertex");

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    shaderProgram.disableAttributeArray("vertex");
    shaderProgram.release();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {

    if(event->button() == Qt::LeftButton &&
       event->buttons() && Qt::LeftButton)
    {
        int x = event->x();
        int y = event->y();
        if(x >= 0 && x <= width() &&
           y >= 0 && y <= height()  )
        {
            grabMouse();
            grabKeyboard();
            QPoint p( width()/2, height()/2);
            QCursor::setPos( mapToGlobal(p) );
            bInputGrabbed = true;
        }
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {

    int x = event->x();
    int y = event->y();
    if(x >= 0 && x <= width() &&
       y >= 0 && y <= height()  )
    {
        if(bInputGrabbed)
        {
            QPoint p( width()/2, height()/2);
            QCursor::setPos( mapToGlobal(p) );

            QPoint Delta = mapToGlobal(event->pos()) - mapToGlobal(p);
            //cout << Delta.x() << " " << Delta.y() << endl;
            /*PreviousMousePos = QCursor::pos();

            alpha -= Delta.x();

            while (alpha < 0)
            {
                alpha += 360;
            }
            while (alpha >= 360)
            {
                alpha -= 360;
            }
            beta -= Delta.y();
            if (beta < -90) {
                beta = -90;

            }
            if (beta > 90)
            {
                beta = 90;
            }*/
            //cout << alpha << " " << beta << endl;
            //UpdateCameraAngles(Delta.y(), -Delta.x(), 0.0);

            UpdateCamera(-Delta.y(), -Delta.x());
            updateGL();
        }
    }
    else
    {
        //managed to move outside the rendering window
        //probably by moving the mouse too fast
        if(bInputGrabbed)
        {
            releaseMouse();
            releaseKeyboard();
            bInputGrabbed = false;
            cout << "Lost Mouse" << endl;
        }
    }
}

void GLWidget::keyPressEvent(QKeyEvent* event)
{




    switch(event->key())
    {
    case Qt::Key_Escape:
        releaseMouse();
        releaseKeyboard();
        bInputGrabbed = false;
        //cout << "released!" << endl;
        break;
    case Qt::Key_Space:

        break;
    case Qt::Key_Up:
    case Qt::Key_W:
        MoveCamera(true,false,false,false);
        updateGL();
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        MoveCamera(false,true,false,false);
        updateGL();
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        MoveCamera(false,false,true,false);
        updateGL();
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        MoveCamera(false,false,false,true);
        updateGL();
        break;
    default:
        event->ignore();
        break;
    }
}
