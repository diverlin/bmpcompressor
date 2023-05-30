QT += widgets

INCLUDEPATH += $$PWD/../lib
LIBS += -L$$OUT_PWD/../lib -llibbpmcoder

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

TARGET = bmpcoderui

# install
INSTALLS += target

FORMS += \
    ui/messagebox.ui
