#-------------------------------------------------
#
# Project created by QtCreator 2015-11-16T15:17:43
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DuSQLiteManagerPlus
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    AjustesDialog.cpp \
    EditorTextoSettings.cpp \
    Application.cpp

HEADERS  += MainWindow.h \
    AjustesDialog.h \
    EditorTextoSettings.h \
    Application.h

FORMS    += MainWindow.ui \
    AjustesDialog.ui

QMAKE_CXXFLAGS += -std=gnu++14

RESOURCES += \
    res.qrc

RC_ICONS = appicon.ico
