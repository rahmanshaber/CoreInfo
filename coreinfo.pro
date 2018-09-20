QT       += core gui widgets

TARGET = coreinfo
TEMPLATE = app

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

# library for theme
unix:!macx: LIBS += -lcprime

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

# Disable warnings, enable threading support and c++11
CONFIG += thread silent build_all c++11

# Disable Debug on Release
# CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT

# Build location

BUILD_PREFIX = $$(CA_BUILD_DIR)

isEmpty( BUILD_PREFIX ) {
        BUILD_PREFIX = ./build
}

MOC_DIR       = $$BUILD_PREFIX/moc-qt5
OBJECTS_DIR   = $$BUILD_PREFIX/obj-qt5
RCC_DIR	      = $$BUILD_PREFIX/qrc-qt5
UI_DIR        = $$BUILD_PREFIX/uic-qt5


unix {
        isEmpty(PREFIX) {
                PREFIX = /usr
        }
        BINDIR = $$PREFIX/bin

        target.path = $$BINDIR

        desktop.path = $$PREFIX/share/applications/
        desktop.files = "CoreInfo.desktop"

        icons.path = $$PREFIX/share/coreapps/icons/
        icons.files = icons/CoreInfo.svg

        INSTALLS += target icons desktop
}

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += "HAVE_POSIX_OPENPT"
DEFINES += _UNICODE

RESOURCES += \
    icons.qrc
