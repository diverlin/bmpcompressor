TEMPLATE = lib

HEADERS += \
    $$PWD/rawimagedata.h \
    $$PWD/bmpreader.h \
    $$PWD/bmpcoder.h \

SOURCES += \
    $$PWD/bmpreader.cpp \
    $$PWD/bmpcoder.cpp \

TARGET = libbpmcoder

INSTALLS += target
