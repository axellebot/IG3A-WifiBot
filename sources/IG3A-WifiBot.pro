#-------------------------------------------------
#
# Project created by QtCreator 2018-04-26T19:17:51
#
#-------------------------------------------------

QT       += core gui network opengl multimedia multimediawidgets webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IG3A-WifiBot
TEMPLATE = app


SOURCES += main.cpp\
    wifibotmanager.cpp \
    mainwindow.cpp \
    connectiondialog.cpp

HEADERS  += \
    wifibotmanager.h \
    mainwindow.h \
    connectiondialog.h

FORMS    += mainwindow.ui \
    connectiondialog.ui

DISTFILES +=

RESOURCES += \
    projectressources

message("OpenCV path: $$OPENCV_PATH")
message("Includes path: $$INCLUDEPATH")
message("Libraries: $$LIBS")
