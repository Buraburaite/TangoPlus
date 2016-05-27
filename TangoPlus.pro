#-------------------------------------------------
#
# Project created by QtCreator 2015-01-12T01:15:33
#
#-------------------------------------------------

QT       += core gui multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TangoPlus
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    videowidget.cpp \
    textedit.cpp \
    doubtitles.cpp \
    playwidget.cpp \
    customvideo.cpp

HEADERS  += mainwindow.h \
    videowidget.h \
    textedit.h \
    doubtitles.h \
    playwidget.h \
    customvideo.h

FORMS    += mainwindow.ui

DISTFILES += \
    Dubtitles.py

CONFIG += static
