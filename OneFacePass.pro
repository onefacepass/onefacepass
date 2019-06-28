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

#DEFINES += _DEBUG


INCLUDEPATH     += $$PWD \
                    $$PWD/form  \
                    $$PWD/demo  \
                    C:\opencv\build\include\    \
                    C:\ArcSoft\inc  \
                    $$PWD/face_recognition/face_module/inc    \
                    $$PWD/face_recognition/third_party/jsoncpp/inc

CONFIG          += qt warn_off

LIBS            += C:\ArcSoft\lib\X64\libarcsoft_face_engine.lib             \
                    C:\opencv\build\x64\vc15\lib\opencv_world410d.lib        \
#                    $$PWD/face_recognition/x64/Debug/face_module.lib         \
                    C:\Users\haoha\source\repos\face_test\x64\Debug\face_test.lib \
                    $$PWD/face_recognition/third_party/jsoncpp/lib/jsoncpp.lib



SOURCES += main.cpp\
    form/cameraviewfinder.cpp \
    form/capturethread.cpp \
    form/quiwidget.cpp \
    form/quicreator.cpp \
    demo/appinit.cpp
#    face_recognition/face_module/src/*.cpp



HEADERS  += head.h\
    form/cameraviewfinder.h \
    form/capturethread.h \
    form/quiwidget.h \
    form/quicreator.h \
    demo/appinit.h  \
    face_recognition/face_module/inc/FaceDete.h
#    face_recognition/face_module/inc/*.h



FORMS    += \
    form/quicreator.ui


RESOURCES += \
    other/qss.qrc \
    other/main.qrc

