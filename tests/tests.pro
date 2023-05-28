QT += testlib

INCLUDEPATH += $$PWD/../lib

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/bmpcodertest.cpp \
    $$PWD/bmpreadertest.cpp \

HEADERS += \
    $$PWD/bmpcodertest.h \
    $$PWD/bmpreadertest.h \

LIBS += -L$$OUT_PWD/../lib -llibbpmcoder

RESOURCES += \
    tests.qrc

TARGET = bmpcodertest



