include(../common.pri)

TEMPLATE = lib
TARGET = $${NAME}qml
TARGET = $$qtLibraryTarget($$TARGET)

INCLUDEPATH += ../lib/
!CONFIG(selfcontained): {
LIBS += -L../lib -l$${NAME}
}

QT += gui qml
CONFIG += plugin

OTHER_FILES += qmldir
import.files = qmldir

import.path = $$QMLDIR
target.path = $$QMLDIR

CONFIG(selfcontained): {
include(../lib/lib.pri)
}

SOURCES += plugin.cpp

INSTALLS += target import
