TEMPLATE = app
TARGET = qtziper

mac {
  CONFIG -= app_bundle
}

QT = core

SOURCES += main.cpp \
    ../../source/FolderCompressor/FolderCompressor.cpp

HEADERS += \
    ../../source/FolderCompressor/FolderCompressor.h

INCLUDEPATH += ../../source/FolderCompressor
