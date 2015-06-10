TEMPLATE = app
TARGET   = modeleur-bmesh
CONFIG  += qt opengl warn_on debug
QT *= xml opengl 
INCLUDEPATH += src include Eigen/ nlopt/include/
DEPENDPATH += src include

QMAKE_CXXFLAGS += -O3 -frounding-math
DEFINES += EIGEN_NO_DEBUG

HEADERS  = include/*.h
SOURCES  = src/*.cpp

macx:LIBS *= -framework Glut
linux-g++*:LIBS *= -lglut -lOpenMeshCored -lGLU -Lnlopt/lib -lnlopt -lCGAL -lgmp
# MacOS, default
QGL = -lqglviewer

# Some Ubuntu variants
exists( /usr/lib/libqglviewer-qt4.so): QGL = -lqglviewer-qt4
exists( /usr/lib/x86_64-linux-gnu/libqglviewer-qt4.so ): QGL = -lqglviewer-qt4

# Ubuntu 14.04 & libqglviewer2
exists( /usr/lib/x86_64-linux-gnu/libQGLViewer.so.2 ): QGL=-lQGLViewer -lGLU

# CentOS
exists( /usr/lib64/libQGLViewer.so ): QGL=-lQGLViewer

LIBS *= $${QGL}
