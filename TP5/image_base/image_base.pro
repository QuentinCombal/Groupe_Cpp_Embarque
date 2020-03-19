#-------------------------------------------------
#
# Project created by QtCreator 2015-01-07T21:40:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = triangle
TEMPLATE = app


SOURCES += main.cpp\
    colorgradient.cpp \
    mainwindow.cpp \
    mandelbrot_fractal.cpp \
    splineimage.cpp


HEADERS  += mainwindow.h \
  colorgradient.h \
  commify.hpp \
  mandelbrot_fractal.h \
  spline.hpp \
  splineimage.h

QMAKE_CXXFLAGS += -std=c++14 -mavx
INCLUDEPATH += /usr/local/include/eigen
