#include "glwidget.h"
#include <QtGui/QMouseEvent>
#include <QTimer>
#include <iostream>
using namespace std;

#include "VectorHelpers.h"


GLWidget::GLWidget(QWidget *parent) :  QGLWidget(parent)
{
    setMouseTracking(true);
    bInputGrabbed = false;
    PreviousMousePos = QCursor::pos();

    theta = 90;
    phi =   0;
    roll = 0;

    CameraPosition = QVector3D(0,0,1.0);
    CameraDirection = QVector3D(1,0,0);
    CameraUp = QVector3D(0,0,1);

    UpdateCamera(0.0,0.0,0.0);

    bForwardKey = bBackwardKey = bLeftKey = bRightKey = false;

    InputTickTimer = new QTimer(this);
    msPerFrame = 33;  //aim for about 30fps
    InputTickTimer->setInterval(msPerFrame);
    connect(InputTickTimer, SIGNAL(timeout()), this, SLOT(InputTimer()));
    setAutoFillBackground(false);
}

GLWidget::~GLWidget()
{
    vertices.clear();
    vertices.squeeze();
    VertexColours.clear();
    VertexColours.squeeze();
}


bool GLWidget::HasGrabbedInput()
{
    return bInputGrabbed;
}

QPoint GLWidget::MapFromScreenspaceToLocal(double x, double y)
{
    int w = width();
    int h = height();

    QPoint P;
    P.rx() = (x*0.5 + 0.5)*w;
    P.ry() = (-y*0.5 + 0.5)*h;
    return P;
}

void GLWidget::InputTimer()
{
    if(!this->hasFocus())
    {
        GrabInput(false);
    }

    PerformCameraMovement();

    //updateGL();
    update();
}


void GLWidget::initializeGL()
{

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    /*
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);
    qglClearColor(QColor(Qt::black));
    */
    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, ":/vertexShader.vsh");
    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, ":/fragmentShader.fsh");
    shaderProgram.link();
    vertices << QVector4D(1, 0, -2, 1) << QVector4D(0, 1, -2, 1) << QVector4D(-3, 0, -2, 1);
    vertices << QVector4D(1, 2, 0, 1) << QVector4D(0, 2, 1, 1) << QVector4D(-1, 2, 0, 1);

    VertexColours << QVector3D(1,0,0) << QVector3D(1,0,0) << QVector3D(1,0,0);
    VertexColours << QVector3D(0,1,0) << QVector3D(0,1,0) << QVector3D(0,1,0);

    vertices << QVector4D(1, 0, 2, 1) << QVector4D(0, 1, 2, 1) << QVector4D(-3, 0, 2, 1);
    vertices << QVector4D(1, -2, 0, 1) << QVector4D(0, -2, 1, 1) << QVector4D(-1, -2, 0, 1);

    VertexColours << QVector3D(0,0,1) << QVector3D(0,0,1) << QVector3D(0,0,1);
    VertexColours << QVector3D(1,1,1) << QVector3D(1,1,1) << QVector3D(1,1,1);
}


void GLWidget::resizeGL(int width, int height)
{

    if (height == 0) {
        height = 1;
    }

    ProjectionMatrix.setToIdentity();
    ProjectionMatrix.perspective(60.0, (float) width / (float) height, 0.001f, 1000.0f);
    glViewport(0, 0, width, height);

}


QMatrix4x4 MakeMatrixRotationYawPitchRoll( float roll, float pitch, float yaw )
{
    QMatrix4x4 out;
    out.setToIdentity();
    out.rotate(yaw,0,1,0);
    out.rotate(roll,1,0,0);
    out.rotate(pitch,0,0,1);
    return out;
}


void GLWidget::UpdateCamera(double delta_theta, double delta_phi, double delta_roll)
{
/*
    theta += delta_theta;
    phi += delta_phi;
    roll += delta_roll;

    if(theta > 360) theta -= 360;
    if(theta < 0)   theta += 360;

    if(phi > 360) phi -= 360;
    if(phi < 0)   phi += 360; 

    if(roll > 360) roll -= 360;
    if(roll < 0)   roll += 360;

    QMatrix4x4 rotMatrix = MakeMatrixRotationYawPitchRoll(theta,roll,phi);
    CameraDirection = rotMatrix * QVector3D(0,0,1);
    CameraDirection.normalize();
    CameraUp    = rotMatrix * QVector3D(0,-1,0);
    CameraUp.normalize();
    CameraTarget = CameraPosition + 1.0*CameraDirection;

    ViewMatrix.setToIdentity();
    ViewMatrix.lookAt(CameraPosition,CameraTarget,CameraUp);

*/

 //this one has probelms when theta goes < 0  or > 180

    CameraDirection = QVector3D(1,0,0);
    CameraUp = QVector3D(0,0,1);

    QMatrix4x4 rotMatrix;

    theta += delta_theta;
    phi += delta_phi;
    roll += delta_roll;

    if(theta > 90) theta = 90;
    if(theta < -90)   theta = -90;

    if(phi > 360) phi -= 360;
    if(phi < 0)   phi += 360;


    QVector3D vLeft = QVector3D::crossProduct(CameraUp,CameraDirection);
    vLeft.normalize();

    rotMatrix.setToIdentity();
    //rotMatrix.rotate(delta_theta,vLeft);
    rotMatrix.rotate(theta,vLeft);

    CameraDirection = (rotMatrix*CameraDirection).normalized();
    CameraUp        = (rotMatrix*CameraUp).normalized();


    rotMatrix.setToIdentity();
    //rotMatrix.rotate(delta_phi, QVector3D(0,0,1));
    rotMatrix.rotate(phi, QVector3D(0,0,1));

    CameraDirection = (rotMatrix*CameraDirection).normalized();
    CameraUp        = (rotMatrix*CameraUp).normalized();

    CameraTarget    = CameraPosition + 1.0*CameraDirection;
    ViewMatrix.setToIdentity();
    ViewMatrix.lookAt(CameraPosition,CameraTarget,CameraUp);

/*
    theta += delta_theta;
    phi += delta_phi;
    roll += delta_roll;

    theta = std::min(theta, 180.0);
    theta = std::max(theta,0.0); //clamp theta

    if(phi > 360) phi -= 360;
    if(phi < 0)   phi += 360;


    CameraDirection = CreateSphericalVector(1.0, Deg2Rad(theta), Deg2Rad(phi) );

    QMatrix4x4 rotMatrix;

    QVector3D vLeft = QVector3D::crossProduct(CameraUp,CameraDirection);
    vLeft.normalize();

    rotMatrix.rotate(theta, vLeft);
    rotMatrix.rotate(phi, QVector3D(0,0,1) );

    CameraUp = rotMatrix*QVector3D(0,0,0);

    CameraTarget    = CameraPosition + 1.0*CameraDirection;

    ViewMatrix.setToIdentity();
    ViewMatrix.lookAt(CameraPosition,CameraTarget,CameraUp);*/
}

void GLWidget::MoveCamera(QVector3D Delta)
{
    CameraPosition += Delta;
}


void GLWidget::SetMovementKey(GLWidget::MovementKey Key, bool bStatus)
{
    switch(Key)
    {
    case Forward:
        bForwardKey = bStatus;
        break;
    case Backward:
        bBackwardKey = bStatus;
        break;
    case Left:
        bLeftKey = bStatus;
        break;
    case Right:
        bRightKey = bStatus;
        break;
    default:
        cout << "Error: Unknown Movement key" << endl;
    }
}

void GLWidget::GrabInput(bool bGrab)
{
    if( bGrab )
    {
        grabMouse();
        grabKeyboard();
        QPoint p( width()/2, height()/2);
        QCursor::setPos( mapToGlobal(p) );
        bInputGrabbed = true;
        InputTickTimer->start();
    }
    else
    {
        releaseMouse();
        releaseKeyboard();
        bInputGrabbed = false;
        InputTickTimer->stop();
        SetMovementKey(Forward,false);
        SetMovementKey(Backward,false);
        SetMovementKey(Left,false);
        SetMovementKey(Right,false);
    }
}

void GLWidget::PerformCameraMovement()
{
    double Speed = 2.0; //"Distance units" per second
    double TimePassed = double(msPerFrame)/1000;
    double Distance = Speed*TimePassed;

    QVector3D MovementVector(0,0,0);
    QVector3D vLeft = QVector3D::crossProduct(CameraUp,CameraDirection);
    vLeft.normalize();
    if(bForwardKey)
    {
        MovementVector += CameraDirection;
    }
    if(bBackwardKey)
    {
        MovementVector -= CameraDirection;
    }
    if(bLeftKey)
    {
        MovementVector += vLeft;
    }
    if(bRightKey)
    {
        MovementVector -= vLeft;
    }
    MovementVector.normalize();

    MovementVector *= Distance;
    MoveCamera(MovementVector);
    UpdateCamera(0.0,0.0,0.0);
}


void GLWidget::paintEvent(QPaintEvent *)
{
    //cout <<"Paint event!" << endl;
    makeCurrent();


    glEnable(GL_MULTISAMPLE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);
    qglClearColor(QColor(Qt::black));

    int w = width();
    int h = height();
    if (h == 0) {
        h = 1;
    }

    ProjectionMatrix.setToIdentity();
    ProjectionMatrix.perspective(60.0, (float) w / (float) h, 0.001f, 1000.0f);
    glViewport(0, 0, w, h);



    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 ModelMatrix;
    ModelMatrix.translate(QVector3D(0,0,0));

    QMatrix4x4 mvpMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;

    shaderProgram.bind();
    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    shaderProgram.setAttributeArray("colour", VertexColours.constData());
    shaderProgram.enableAttributeArray("colour");

    shaderProgram.setAttributeArray("vertex", vertices.constData());
    shaderProgram.enableAttributeArray("vertex");
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    shaderProgram.disableAttributeArray("vertex");
    shaderProgram.release();

    bool bInvertible;
    QMatrix4x4 vpInverse = (ProjectionMatrix * ViewMatrix).inverted(&bInvertible);

    if(!bInvertible)
    {
        cout << "Error: Unable to invert vp matrix" << endl;
        return;
    }

    QVector2D AxisScreenOrigin(-0.9,-0.9); //0,0 in screenspace

    QVector4D AxisOrigin(AxisScreenOrigin.x(),AxisScreenOrigin.y(),1.0,1.0);
    AxisOrigin = vpInverse*AxisOrigin;  //axisorigin is now screenspace(0,0) in worldspace

    QVector4D TempPoint;


    QPoint XLabelPos,YLabelPos,ZLabelPos;

    glLineWidth(2.5);

    glBegin(GL_LINES);

    glColor3f(1.0, 0.0, 0.0);
    TempPoint = AxisOrigin + QVector4D(0.1,0,0,0);
    TempPoint = mvpMatrix*TempPoint;
    glVertex3f(AxisScreenOrigin.x(), AxisScreenOrigin.y(), 0.0);
    glVertex3f(TempPoint.x(), TempPoint.y(), 0);

    XLabelPos = MapFromScreenspaceToLocal(TempPoint.x(),TempPoint.y());

    glColor3f(0.0, 1.0, 0.0);
    TempPoint = AxisOrigin + QVector4D(0,0.1,0,0);
    TempPoint = mvpMatrix*TempPoint;
    glVertex3f(AxisScreenOrigin.x(), AxisScreenOrigin.y(), 0.0);
    glVertex3f(TempPoint.x(), TempPoint.y(), 0);


    YLabelPos = MapFromScreenspaceToLocal(TempPoint.x(),TempPoint.y());

    glColor3f(0.0, 0.0, 1.0);
    TempPoint = AxisOrigin + QVector4D(0,0,0.1,0);
    TempPoint = mvpMatrix*TempPoint;
    glVertex3f(AxisScreenOrigin.x(), AxisScreenOrigin.y(), 0.0);
    glVertex3f(TempPoint.x(), TempPoint.y(), 0);

    ZLabelPos = MapFromScreenspaceToLocal(TempPoint.x(),TempPoint.y());

    glEnd();



    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    QPen pen(QColor(255,255,255));

    QPainter p(this);

    p.setPen(pen);


    p.setRenderHint(QPainter::Antialiasing);
    p.drawText(XLabelPos,"X");
    p.drawText(YLabelPos,"Y");
    p.drawText(ZLabelPos,"Z");

    p.end();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{

    if(event->button() == Qt::LeftButton &&
       event->buttons() && Qt::LeftButton)
    {
        int x = event->x();
        int y = event->y();
        if(x >= 0 && x <= width() &&
           y >= 0 && y <= height()  )
        {
            GrabInput(true);
        }
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(!this->hasFocus())
    {
        GrabInput(false);
    }

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

            UpdateCamera(Delta.y(), -Delta.x(), 0.0);
            //updateGL();
            update();
        }
    }
    else
    {
        //managed to move outside the rendering window
        //probably by moving the mouse too fast
        if(bInputGrabbed)
        {
            GrabInput(false);
        }
    }
}

void GLWidget::keyPressEvent(QKeyEvent* event)
{
    if(!this->hasFocus())
    {
        GrabInput(false);
    }



    switch(event->key())
    {
    case Qt::Key_Escape:
        GrabInput(false);
        break;
    case Qt::Key_Space:
        UpdateCamera(0.0, 0.0, 10.0);
        break;
    case Qt::Key_Up:
    case Qt::Key_W:
        //MoveCamera(true,false,false,false);
        SetMovementKey(Forward,true);
        //updateGL();
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        //MoveCamera(false,true,false,false);
        SetMovementKey(Backward,true);
        //updateGL();
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        //MoveCamera(false,false,true,false);
        SetMovementKey(Left,true);
        //updateGL();
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        //MoveCamera(false,false,false,true);
        SetMovementKey(Right,true);
        //updateGL();
        break;
    default:
        event->ignore();
        break;
    }

}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(!this->hasFocus())
    {
        GrabInput(false);
    }

    switch(event->key())
    {
    case Qt::Key_Up:
    case Qt::Key_W:
        //MoveCamera(true,false,false,false);
        SetMovementKey(Forward,false);
        //updateGL();
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        //MoveCamera(false,true,false,false);
        SetMovementKey(Backward,false);
        //updateGL();
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        //MoveCamera(false,false,true,false);
        SetMovementKey(Left,false);
        //updateGL();
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        //MoveCamera(false,false,false,true);
        SetMovementKey(Right,false);
        //updateGL();
        break;
    default:
        event->ignore();
        break;
    }
}
