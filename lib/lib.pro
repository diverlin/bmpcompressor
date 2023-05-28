TEMPLATE = lib

HEADERS += \
    $$PWD/rawimagedata.h \
    $$PWD/bmpheader.h \
    $$PWD/bmpreader.h \
    $$PWD/bmpwriter.h \
    $$PWD/bmpcoder.h \

SOURCES += \
    $$PWD/bmpreader.cpp \
    $$PWD/bmpwriter.cpp \
    $$PWD/bmpcoder.cpp \

TARGET = libbpmcoder

INSTALLS += target
