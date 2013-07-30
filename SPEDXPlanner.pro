#-------------------------------------------------
#
# Project created by QtCreator 2013-07-09T16:56:08
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = SPEDXPlanner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    mainrenderwindow.cpp \
    crystal.cpp \
    crystallography.cpp \
    spectrum.cpp \
    geometry.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    VectorHelpers.h \
    mainrenderwindow.h \
    crystal.h \
    crystallography.h \
    Utils.h \
    spectrum.h \
    geometry.h

FORMS    += mainwindow.ui \
    mainrenderwindow.ui

mac: LIBS += -F$$PWD/../../../../Qt5.1.0/5.1.0/clang_64/lib/ -framework QtOpenGL

INCLUDEPATH += $$PWD/../../../../Qt5.1.0/5.1.0/clang_64/include
DEPENDPATH += $$PWD/../../../../Qt5.1.0/5.1.0/clang_64/include

OTHER_FILES += \
    vertexShader.vsh \
    fragmentShader.fsh

RESOURCES += \
    shaders.qrc
