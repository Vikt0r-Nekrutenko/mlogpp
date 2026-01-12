TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += \
        headers/

HEADERS += \
        tokenizer.hpp
        
SOURCES += \
        *.cpp
