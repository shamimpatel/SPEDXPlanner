#-------------------------------------------------
#
# Project created by QtCreator 2013-07-09T16:56:08
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SPEDXPlanner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    VectorHelpers.h

FORMS    += mainwindow.ui

mac: LIBS += -F$$PWD/../../../../Qt5.0.2/5.0.2/clang_64/lib/ -framework QtOpenGL

INCLUDEPATH += $$PWD/../../../../Qt5.0.2/5.0.2/clang_64/include
DEPENDPATH += $$PWD/../../../../Qt5.0.2/5.0.2/clang_64/include

OTHER_FILES += \
    vertexShader.vsh \
    fragmentShader.fsh

RESOURCES += \
    shaders.qrc
