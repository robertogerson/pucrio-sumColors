CONFIG   += debug
QT       += core gui svg

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
    src/gui/ViewConfiguration.cpp

HEADERS  +=\
         include/gui/MainWindow.h \
         include/SpectralData.h \
    include/gui/ColorInformationDock.h \
    include/gui/ColorChooseDock.h \
    include/gui/ViewConfiguration.h

FORMS    += \
    ui/colorinformationdock.ui \
    ui/ColorChooseDock.ui \
    ui/ViewConfiguration.ui
