QT += testlib

include($$PWD/../lib/lib.pri)

INCLUDEPATH += $$PWD/../lib

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/bmpcompressortest.cpp \

HEADERS += \
    $$PWD/bmpcompressortest.h \

TARGET = bmpcompressortest




