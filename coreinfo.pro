#-------------------------------------------------
#
# Project created by QtCreator 2018-08-13T08:34:24
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = coreinfo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        coreinfo.cpp \
    configtreetext.cpp \
    Core.cpp

HEADERS += \
        coreinfo.h \
    configtreetext.h \
    Core.h

!defined(packagesExist, test) {
    defineTest(packagesExist) {
        system(pkg-config $$ARGS): return(true)
        return(false)
    }
}

unix {
    exists(../../../../MediaInfoLib/Project/GNU/Library/libmediainfo-config) {
        INCLUDEPATH += ../../../../MediaInfoLib/Source
        contains(STATIC_LIBS, yes|1) {
            LIBS += $$system(../../../../MediaInfoLib/Project/GNU/Library/libmediainfo-config LIBS_Static)
            message("custom libmediainfo: yes (static)")
        } else {
            LIBS += $$system(../../../../MediaInfoLib/Project/GNU/Library/libmediainfo-config LIBS)
            message("custom libmediainfo: yes (shared)")
        }
    } else {
        !packagesExist(libmediainfo) {
            error("libmediainfo not found on system")
        }
        LIBS += $$system(pkg-config --libs libmediainfo)
    }

    exists(../../../../ZenLib/Project/GNU/Library/libzen-config) {
        INCLUDEPATH      += ../../../../ZenLib/Source
        contains(STATIC_LIBS, yes|1) {
            LIBS += $$system(../../../../ZenLib/Project/GNU/Library/libzen-config LIBS_Static)
            message("custom libzen       : yes (static)")
        } else {
            LIBS += $$system(../../../../ZenLib/Project/GNU/Library/libzen-config LIBS)
            message("custom libzen       : yes (shared)")
        }
    } else {
        PKGCONFIG += libzen
        message("libzen      : system")
    }
}

unix:LIBS += -ldl \
              -lz

DEFINES += _UNICODE

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
