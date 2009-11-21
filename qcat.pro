TEMPLATE = subdirs 
SUBDIRS = src\logic_and_gui \
          src\test

# build must be last:
CONFIG += ordered
SUBDIRS += src\build
