QT += core widgets

CONFIG += c++17
CONFIG += console
CONFIG -= app_bundle

TARGET = calculator
TEMPLATE = app

SOURCES += \
    main.cpp \
    calculator.cpp

HEADERS += \
    calculator.h

# For Qt Designer UI (optional)
# FORMS += calculator.ui

# Enable MOC for signals/slots
QT += core widgets
