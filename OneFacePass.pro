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
#win32:RC_FILE   = other/main.rc
PRECOMPILED_HEADER  = head.h

DEFINES += _DEBUG


INCLUDEPATH     +=  $$PWD        \
                    $$PWD/form  \
                    $$(OPENCV)/include/    \
                    $$(ARCSOFT)/inc              \
                    $$PWD/3rdparty/face_module/include   \
                    $$PWD/3rdparty/jsoncpp/include

CONFIG          += qt warn_off

LIBS            +=  $$(OPENCV)/x64/vc15/lib/opencv_world346d.lib   \
                    $$PWD/3rdparty/face_module/lib/face_module.lib     \
                    $$(ARCSOFT)/lib/X64/libarcsoft_face_engine.lib      \
                    $$PWD/3rdparty/jsoncpp/lib/jsoncpp.lib



SOURCES += main.cpp\
    form/cameraviewfinder.cpp \
    form/capturethread.cpp \
    form/quiwidget.cpp \
    form/quicreator.cpp \
    form/appinit.cpp



HEADERS  += head.h\
    form/cameraviewfinder.h \
    form/capturethread.h \
    form/quiwidget.h \
    form/quicreator.h \
    form/appinit.h



FORMS    += \
    form/quicreator.ui


RESOURCES += \
    other/qss.qrc \
    other/main.qrc
