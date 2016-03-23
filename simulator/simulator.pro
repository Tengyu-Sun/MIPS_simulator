#-------------------------------------------------
#
# Project created by QtCreator 2016-02-12T13:22:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simulator
TEMPLATE = app


SOURCES += main.cpp\
        simulator.cpp \
    memsys.cpp \
    memory.cpp \
    cache.cpp \
    cpu.cpp \
    fpu.cpp \
    vu.cpp

HEADERS  += simulator.h \
    memsys.h \
    cache.h \
    memory.h \
    memcache.h \
    cpu.h \
    fpu.h \
    vu.h

CONFIG += c++11

#FORMS    += simulator.ui
