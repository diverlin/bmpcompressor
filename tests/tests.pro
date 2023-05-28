QT += testlib

INCLUDEPATH += $$PWD/../lib

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/bmpcompressortest.cpp \
    $$PWD/bmploadertest.cpp \

HEADERS += \
    $$PWD/bmpcompressortest.h \
    $$PWD/bmploadertest.h \

LIBS += -L$$OUT_PWD/../lib -llibbpmcompressor

RESOURCES += \
    tests.qrc

TARGET = bmpcompressortest



