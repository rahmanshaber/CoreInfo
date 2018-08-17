QT       += core gui widgets

TARGET = coreinfo
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += _UNICODE

CONFIG += c++11

SOURCES += main.cpp configtreetext.cpp  Core.cpp coreinfo.cpp
HEADERS += configtreetext.h Core.h coreinfo.h
FORMS += coreinfo.ui

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

unix:LIBS += -ldl -lz

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

