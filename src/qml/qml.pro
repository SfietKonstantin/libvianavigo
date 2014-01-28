include(../common.pri)

TEMPLATE = lib
TARGET = $${NAME}qml
TARGET = $$qtLibraryTarget($$TARGET)

LIBS += -L../lib -l$${NAME}
INCLUDEPATH += ../lib/

QT += gui qml
CONFIG += plugin

OTHER_FILES += qmldir
import.files = qmldir

import.path = $$QML_MODULE_FOLDER
target.path = $$QML_MODULE_FOLDER

SOURCES += plugin.cpp

INSTALLS += target import
