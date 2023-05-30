QT += testlib

INCLUDEPATH += $$PWD/../lib
LIBS += -L$$OUT_PWD/../lib -llibbpmcoder

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/bmpcodertest.cpp \
    $$PWD/bmploadertest.cpp \
    $$PWD/barchloadertest.cpp \
    $$PWD/encodedimagedatatest.cpp \
    $$PWD/helper.cpp \

HEADERS += \
    $$PWD/bmpcodertest.h \
    $$PWD/bmploadertest.h \
    $$PWD/barchloadertest.h \
    $$PWD/encodedimagedatatest.h \
    $$PWD/helper.h \



RESOURCES += \
    tests.qrc

TARGET = bmpcodertest



