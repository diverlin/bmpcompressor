TEMPLATE = lib

HEADERS += \
    $$PWD/rawimagedata.h \
    $$PWD/bmploader.h \
    $$PWD/bmpcompressor.h \

SOURCES += \
    $$PWD/bmploader.cpp \
    $$PWD/bmpcompressor.cpp \

TARGET = libbpmcompressor

INSTALLS += target
