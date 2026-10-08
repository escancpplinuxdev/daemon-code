# myclass.pro
QT += core

CONFIG += c++17 console
CONFIG -= app_bundle

SOURCES += \
    main.cpp \
    myclass.cpp

HEADERS += \
    myclass.h

# MOC will automatically run on HEADERS
# .moc directory will contain moc_myclass.cpp
