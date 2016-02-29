#-------------------------------------------------
#
# Project created by QtCreator 2014-11-17T21:30:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = finall
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    body.cpp \
    bullet.cpp \
    mylawn.cpp \
    plant.cpp \
    zombie.cpp

HEADERS  += mainwindow.h \
    body.h \
    bullet.h \
    mylawn.h \
    plant.h \
    zombie.h

FORMS    += mainwindow.ui

SUBDIRS += \
    final_project.pro

OTHER_FILES += \
    level-1.png \
    level-2.png \
    level-3.png \
    level-4.png \
    level-5.png \
    level-6.png \
    level-7.png \
    level-8.png \
    final_project.pro.user \
    finall.pro.user \
    pvz_levels.csv \
    pvz_users.csv
