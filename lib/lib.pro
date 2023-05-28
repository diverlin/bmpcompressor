TEMPLATE = lib

HEADERS += \
    $$PWD/rawimagedata.h \
    $$PWD/bmploader.h \
    $$PWD/bmpcoder.h \

SOURCES += \
    $$PWD/bmploader.cpp \
    $$PWD/bmpcoder.cpp \

TARGET = libbpmcoder

INSTALLS += target
