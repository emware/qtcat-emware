#Includes common configuration for all subdirectory .pro files.
INCLUDEPATH += . ..

TEMPLATE = lib
CONFIG += staticlib

CONFIG += warn_on \
          qt \
          thread

QT += core

INCLUDEPATH += ./GeneratedFiles ./

CONFIG(debug, debug|release) {
    DESTDIR = Debug
    INCLUDEPATH += ./GeneratedFiles/Debug
    INCLUDEPATH -= ./GeneratedFiles/Release
    MOC_DIR = ./GeneratedFiles/Debug
    OBJECTS_DIR += Debug
    OBJECTS_DIR -= Release
} else {
    DESTDIR = Release
    INCLUDEPATH += ./GeneratedFiles/Release
    INCLUDEPATH -= ./GeneratedFiles/Debug
    MOC_DIR = ./GeneratedFiles/Release
    OBJECTS_DIR += Release
    OBJECTS_DIR -= Debug
}

UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

