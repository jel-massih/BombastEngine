#-------------------------------------------------
#
# Project created by QtCreator 2016-01-24T21:20:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BombastEditor
TEMPLATE = app

INCLUDEPATH += ../../../Engine/Source/ ../../../Engine/Source/Libraries/lua-5.2.3/include

SOURCES += ../BombastEditor/bombasteditor.cpp \
    ../BombastEditor/BombastEditorLogic.cpp \
    ../BombastEditor/bombasteditormain.cpp \
    ../BombastEditor/BombastEditorView.cpp \
    ../Application/MessageHandler.cpp

HEADERS  += ../BombastEditor/bombasteditor.h \
    ../BombastEditor/BombastEditorLogic.h \
    ../BombastEditor/bombasteditormain.h \
    ../BombastEditor/BombastEditorView.h \
    ../Application/MessageHandler.h

FORMS    += ../UiViews/bombasteditormain.ui
