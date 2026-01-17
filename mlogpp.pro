TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += \
        headers/ \
        sources/ \

DEPENDPATH += \
        headers/ \
        sources/ \

HEADERS += \
        headers/syntax_error_handler.hpp \
        headers/tokenizer.hpp \
        
SOURCES += \
        sources/syntax_error_handler.cpp \
        main.cpp \
    sources/tokenizer.cpp
