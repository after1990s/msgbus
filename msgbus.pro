TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    messagebus.cpp

HEADERS += \
    function_traits.h \
    messagebus.h

QMAKE_CXXFLAGS += /std:c++latest
