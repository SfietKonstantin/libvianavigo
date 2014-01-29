include(../common.pri)

TEMPLATE = lib
TARGET = $${NAME}qml
TARGET = $$qtLibraryTarget($$TARGET)

!CONFIG(selfcontained): {
LIBS += -L../lib -l$${NAME}
INCLUDEPATH += ../lib/
}

QT += gui qml
CONFIG += plugin

OTHER_FILES += qmldir
import.files = qmldir

import.path = $$QMLDIR
target.path = $$QMLDIR

CONFIG(selfcontained): {
INCLUDEPATH += ../src
include(../lib/lib.pri)
}

SOURCES += plugin.cpp

INSTALLS += target import
