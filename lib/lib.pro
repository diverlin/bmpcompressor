TEMPLATE = lib

HEADERS += \
    $$PWD/rawimagedata.h \
    $$PWD/encodedimagedata.h \
    $$PWD/bmploader.h \
    $$PWD/barchloader.h \
    $$PWD/bmpcoder.h \
    $$PWD/utils.h \

SOURCES += \
    $$PWD/bmploader.cpp \
    $$PWD/barchloader.cpp \
    $$PWD/bmpcoder.cpp \
    $$PWD/utils.cpp \

TARGET = libbpmcoder

INSTALLS += target
