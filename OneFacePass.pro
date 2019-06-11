#-------------------------------------------------
#
# Project created by QtCreator 2017-02-19T12:55:42
#
#-------------------------------------------------

QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET          = OneFacePass
TEMPLATE        = app
MOC_DIR         = temp/moc
RCC_DIR         = temp/rcc
UI_DIR          = temp/ui
OBJECTS_DIR     = temp/obj
DESTDIR         = $$PWD/bin
win32:RC_FILE   = other/main.rc
PRECOMPILED_HEADER  = head.h

INCLUDEPATH     += $$PWD
INCLUDEPATH     += $$PWD/form
INCLUDEPATH     += $$PWD/demo
CONFIG          += qt warn_off
#DEFINES         += DEBUG

SOURCES += main.cpp\    
    form/quiwidget.cpp \
    form/quicreator.cpp \
    form/panelwidget.cpp \
    form/frmdevice.cpp \
    demo/appinit.cpp    \
#    demo/sliderbar.cpp \
#    demo/navlistview.cpp \
    form/camera.cpp


HEADERS  += head.h\
    form/quiwidget.h \
    form/quicreator.h \
    form/panelwidget.h \
    form/frmdevice.h \
    demo/appinit.h  \
#    demo/sliderbar.h \
#    demo/navlistview.h \
    form/camera.h


FORMS    += \
    form/quicreator.ui \
    form/frmdevice.ui


RESOURCES += \
    other/qss.qrc \
    other/main.qrc

