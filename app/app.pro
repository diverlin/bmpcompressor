QT += widgets

INCLUDEPATH += $$PWD/../lib
LIBS += -L$$OUT_PWD/../ -llibbpmcoder

HEADERS += \
    mainwindow.h \
    filesmodel.h \
    messagebox.h \
    jobfactory.h \

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    filesmodel.cpp \
    messagebox.cpp \
    jobfactory.cpp \

FORMS += \
    ui/messagebox.ui

TARGET = bmpcoderui

DESTDIR = $$OUT_PWD/..

INSTALLS += target


