#-------------------------------------------------
#
# Project created by QtCreator 2018-02-02T16:04:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = android-helper
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    tools/adbtool.cpp \
    centralwidget.cpp \
    deviceinfo.cpp \
    filepage.cpp \
    leftsidebar.cpp \
    main.cpp \
    mainwindow.cpp \
    musicpage.cpp \
    navlistitem.cpp \
    navlistview.cpp \
    rightstackwidget.cpp \
    splitline.cpp \
    tools/deviceprovider.cpp \
    tools/device.cpp \
    currentdevicemanager.cpp \
    devicepage.cpp \
    tools/mtptool.cpp \
    filepagefiletree.cpp \
    tools/findmmfiles.cpp \
    tools/findmmfilesworker.cpp \
    musicpagemusics.cpp \
    musicpagemusicitem.cpp \
    toolsbar.cpp \
    musicpagenav.cpp \
    musicpageartist.cpp \
    musicpagealbum.cpp \
    musicpagedirs.cpp \
    musicplayer.cpp \
    filepagefiletreesd.cpp \
    filepageswitchstorage.cpp \
    tools/taglibtool.cpp \
    filesystemmodelfixed.cpp

HEADERS += \
    tools/adbtool.h \
    centralwidget.h \
    deviceinfo.h \
    filepage.h \
    leftsidebar.h \
    mainwindow.h \
    musicpage.h \
    navlistitem.h \
    navlistview.h \
    rightstackwidget.h \
    splitline.h \
    tools/deviceprovider.h \
    tools/device.h \
    currentdevicemanager.h \
    devicepage.h \
    tools/mtptool.h \
    filepagefiletree.h \
    tools/findmmfiles.h \
    tools/findmmfilesworker.h \
    musicpagemusics.h \
    musicpagemusicitem.h \
    toolsbar.h \
    musicpagenav.h \
    musicpageartist.h \
    musicpagealbum.h \
    musicpagedirs.h \
    musicplayer.h \
    filepagefiletreesd.h \
    filepageswitchstorage.h \
    tools/taglibtool.h \
    filesystemmodelfixed.h

unix:!macx: LIBS += -L/usr/lib/x86_64-linux-gnu/ -ldtkcore
INCLUDEPATH += /usr/include/libdtk-2.0/DCore
DEPENDPATH += /usr/include/libdtk-2.0/DCore

unix:!macx: LIBS += -L/usr/lib/x86_64-linux-gnu/ -ldtkwidget
INCLUDEPATH += /usr/include/libdtk-2.0/DWidget
DEPENDPATH += /usr/include/libdtk-2.0/DWidget

unix:!macx: LIBS += -L/usr/lib/x86_64-linux-gnu/ -ltag
INCLUDEPATH += /usr/include/taglib
DEPENDPATH += /usr/include/taglib

RESOURCES += \
    images.qrc

DISTFILES += \
    README.md
