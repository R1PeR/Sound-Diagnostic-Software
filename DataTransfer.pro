#-------------------------------------------------
#
# Project created by QtCreator 2018-11-24T11:41:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = DataTransfer
TEMPLATE = app
INCLUDEPATH += $$PWD/core
INCLUDEPATH += $$PWD/fft
INCLUDEPATH += $$PWD/mfcc
INCLUDEPATH += $$PWD/onset-detection-functions
INCLUDEPATH += $$PWD/pitch
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
QMAKE_CXXFLAGS += -DUSE_KISS_FFT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    Gist.cpp \
    chart.cpp \
    core/CoreFrequencyDomainFeatures.cpp \
    core/CoreTimeDomainFeatures.cpp \
    fft/WindowFunctions.cpp \
    kiss_fft.c \
    main.cpp \
    mainwindow.cpp \
    mfcc/MFCC.cpp \
    onset-detection-functions/OnsetDetectionFunction.cpp \
    pitch/Yin.cpp \
    thread.cpp

HEADERS += \
    Gist.h \
    _kiss_fft_guts.h \
    chart.h \
    core/CoreFrequencyDomainFeatures.h \
    core/CoreTimeDomainFeatures.h \
    fft/WindowFunctions.h \
    kiss_fft.h \
    kissfft.hh \
    mainwindow.h \
    mfcc/MFCC.h \
    onset-detection-functions/OnsetDetectionFunction.h \
    pitch/Yin.h \
    thread.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES +=
