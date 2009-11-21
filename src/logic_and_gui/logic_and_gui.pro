SOURCES += main.cpp \
mainwindow.cpp \
configmanager.cpp \
datamodule.cpp \
dbnode.cpp \
defsandtools.cpp \
rightpanel.cpp \
catalogtree.cpp \
testcode.cpp \
mediascanner.cpp \
catalogclass.cpp \
mediascanui.cpp \
iconscdcat.cpp \
addscanplaces.cpp \
catalogpropertiesdlg.cpp \
 catalogreports.cpp \
 catalogpassworddlg.cpp \
 iconmanager.cpp \
 iconmanagerdlg.cpp \
 addupdateicondlg.cpp \
 searchdlg.cpp \
 whereisitxmlimportclass.cpp \
 importdlg.cpp \
 progresswidget.cpp \
 deviceclass.cpp \
 catalogtable.cpp
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt \
 build_all \
 debug

HEADERS += mainwindow.h \
configmanager.h \
datamodule.h \
dbnode.h \
defsandtools.h \
rightpanel.h \
catalogtree.h \
testcode.h \
mediascanner.h \
catalogclass.h \
mediascanui.h \
iconscdcat.h \
addscanplaces.h \
catalogpropertiesdlg.h \
 catalogreports.h \
 catalogpassworddlg.h \
 iconmanager.h \
 iconmanagerdlg.h \
 addupdateicondlg.h \
 searchdlg.h \
 whereisitxmlimportclass.h \
 importdlg.h \
 progresswidget.h \
 deviceclass.h \
 catalogtable.h
RESOURCES += application.qrc

QT += core \
gui \
xml \
 sql
FORMS += catalog_pass.ui \
catalog_properties.ui \
 searchdialog.ui \
 iconmanagedialog.ui \
 addupdateicon.ui \
 importdb.ui \
 settings_form.ui

DESTDIR = .

DISTFILES += Tasks.txt

OBJECTS_DIR = ../build/

UI_DIR = ../build/

MOC_DIR = ../build/

TARGET = ../bin/qcat



CONFIG -= release

