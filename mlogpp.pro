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
        headers/ast_node.hpp \
        headers/parser.hpp \
        headers/syntax_error_handler.hpp \
        headers/tokenizer.hpp \
        
SOURCES += \
    sources/ast_node.cpp \
    sources/parser.cpp \
        sources/syntax_error_handler.cpp \
        main.cpp \
    sources/tokenizer.cpp
