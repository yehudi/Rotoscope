#-------------------------------------------------
#
# Project created by QtCreator 2015-02-23T16:59:03
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Rotoscope
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    newprojectdialog.cpp \
    qzonedessin.cpp \
    drawview.cpp \
    picturesview.cpp \
    clickablethumb.cpp

HEADERS  += mainwindow.h \
    newprojectdialog.h \
    qzonedessin.h \
    drawview.h \
    picturesview.h \
    clickablethumb.h

FORMS    += mainwindow.ui

DISTFILES += \
    ../Resources/art-studio/artstudiopng/marker.png \
    ../Resources/art-studio/artstudiopng/bricks9.png \
    ../Resources/art-studio/artstudiopng/landscape14.png \
    ../Resources/art-studio/artstudiopng/sun1.png
