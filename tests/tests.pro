QT += testlib

INCLUDEPATH += $$PWD/../lib

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/bmpcodertest.cpp \
    $$PWD/bmploadertest.cpp \

HEADERS += \
    $$PWD/bmpcodertest.h \
    $$PWD/bmploadertest.h \

LIBS += -L$$OUT_PWD/../lib -llibbpmcoder

RESOURCES += \
    tests.qrc

TARGET = bmpcodertest



