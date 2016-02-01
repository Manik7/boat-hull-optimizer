#-------------------------------------------------
#
# Project created by QtCreator 2016-02-01T16:52:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hull_plotter
TEMPLATE = app


SOURCES += main.cpp\
        hullplotter.cpp

HEADERS  += hullplotter.h \
    qStationView.h \
    Station_qt.h \
    Point_3_qt.h \
    Hull_qt.h

FORMS    += hullplotter.ui

#CONFIG += c++11
#gcc:CXXFLAGS += -std=c++11

QMAKE_CXXFLAGS += -std=c++11
