CONFIG   += debug
QT       += core gui svg webkit

TARGET = trabCor
TEMPLATE = app


INCLUDEPATH += /usr/include /usr/local/include /usr/local/include/plplot \
	include include/gui

LIBS += -L/usr/local/lib -lplplotcxxd -lplplotqtd

SOURCES +=\
        src/trabCor.cpp \
        src/gui/MainWindow.cpp \
        src/SpectralData.cpp \
    src/gui/ColorInformationDock.cpp \
    src/gui/ColorChooseDock.cpp \
    src/gui/ViewConfiguration.cpp \
    src/gui/About.cpp

HEADERS  +=\
         include/gui/MainWindow.h \
         include/SpectralData.h \
    include/gui/ColorInformationDock.h \
    include/gui/ColorChooseDock.h \
    include/gui/ViewConfiguration.h \
    include/gui/About.h

FORMS    += \
    ui/colorinformationdock.ui \
    ui/ColorChooseDock.ui \
    ui/ViewConfiguration.ui \
    ui/about.ui
