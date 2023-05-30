QT += widgets

HEADERS += \
    mainwindow.h \
    filesmodel.h \
    messagebox.h \

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    filesmodel.cpp \
    messagebox.cpp \

TARGET = bmpcoderui

# install
INSTALLS += target

FORMS += \
    ui/messagebox.ui
