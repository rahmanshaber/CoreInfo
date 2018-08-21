QT       += core gui widgets

TARGET = coreinfo
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += _UNICODE

CONFIG += c++11

unix:LIBS += -ldl -lz

FORMS += \
    coreinfo.ui

HEADERS += \
    configtreetext.h \
    Core.h \
    coreinfo.h

SOURCES += \
    main.cpp \
    configtreetext.cpp \
    Core.cpp \
    coreinfo.cpp

!defined(packagesExist, test) {
    defineTest(packagesExist) {
        system(pkg-config $$ARGS): return(true)
        return(false)
    }
}

unix:!macx {
     LIBS += $$system(pkg-config --libs libmediainfo)
     PKGCONFIG += libzen
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

