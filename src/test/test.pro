#
# Test modules. Runs with QtTestUtil.
#

INCLUDEPATH += ../logic_and_gui

! include( ../common.pri ) {
    error( Couldn't find the common.pri file! )
}

TEMPLATE = app
CONFIG -= staticlib

include(QtTestUtil/QtTestUtil.pri)

QT += gui \
      xml \
      sql \
      testlib


CONFIG(debug,debug|release) {
 LIBS += -L../logic_and_gui/Debug -llogic_and_gui
}
else {
 LIBS += -L../logic_and_gui/Release -llogic_and_gui
}


# build test executable
TARGET = ../../../bin/test-qcat

CONFIG -= app_bundle
CONFIG += qtestlib


# SimpleChecker.cpp contains the entry point.
SOURCES += \
	testcode.cpp \
	test_create_cat.cpp \
	test_parse_xml.cpp \
	QtTestUtil/SimpleChecker.cpp

HEADERS += \
	testcode.h \
	QtTestUtil/QtTestUtil.h


# Add an extra 'make check' target.
QMAKE_EXTRA_TARGETS = check
check.commands = \$(MAKE) && ./$(QMAKE_TARGET)

# Cleanup the checker on 'make clean'
QMAKE_CLEAN += $(QMAKE_TARGET)
