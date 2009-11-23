#
# build final executable
#

INCLUDEPATH += ../logic_and_gui

! include( ../common.pri ) {
    error( Couldn't find the common.pri file! )
}
TEMPLATE = app
CONFIG -= staticlib

TARGET = ../../../bin/qcat

QT += gui \
      xml \
      sql

CONFIG(debug,debug|release) {
 LIBS += -L../logic_and_gui/Debug -llogic_and_gui
 TARGET = ../../../bin/qcatsym
}
else {
 LIBS += -L../logic_and_gui/Release -llogic_and_gui
}

# main.cpp contains the entry point.
SOURCES += main.cpp

#Resource file(s)
RESOURCES += .\application.qrc

