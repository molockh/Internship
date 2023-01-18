TEMPLATE = app
QT += qml quick quickcontrols2
SOURCES += main.cpp ImageDirectory.cpp
HEADERS += ImageDirectory.h
RESOURCES += qml.qrc

qml.files += $$files(*.qml)
qml.path = "."
INSTALLS += qml
