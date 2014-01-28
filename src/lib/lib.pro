include(../common.pri)

TEMPLATE = lib
CONFIG += qt create_prl no_install_prl create_pc
QT = core network

DEFINES += VIANAVIGO_LIBRARY

TARGET = $${NAME}

HEADERS += vianavigo_global.h \
    searchresult.h \
    searchresult_p.h \
    abstractmodel.h \
    abstractmodel_p.h \
    searchmodel.h \
    osignal.h \
    manager.h

SOURCES += searchresult.cpp \
    abstractmodel.cpp \
    searchmodel.cpp \
    manager.cpp

TARGET = $${NAME}
target.path = $${LIBDIR}

QMAKE_PKGCONFIG_NAME = lib$$TARGET
QMAKE_PKGCONFIG_DESCRIPTION = Vianavigo library
QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_INCDIR = $$headers.path
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

headers.files = $$PWD/*.h
headers.path = $${INCLUDEDIR}

INSTALLS += target headers
