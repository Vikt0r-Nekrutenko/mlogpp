TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

OBJECTS_DIR = objects

INCLUDEPATH += \
        headers/ \
        sources/ \

DEPENDPATH += \
        headers/ \
        sources/ \

HEADERS += \
        headers/ast_assigment.hpp \
        headers/ast_buildin_function_call.hpp \
        headers/ast_cell_access_node.hpp \
        headers/ast_else_node.hpp \
        headers/ast_function_implementation_node.hpp \
        headers/ast_if_node.hpp \
        headers/ast_node.hpp \
        headers/ast_operator_node.hpp \
        headers/ast_variable.hpp \
        headers/parser.hpp \
        headers/syntax_error_handler.hpp \
        headers/tokenizer.hpp

SOURCES += \
    sources/ast_assigment.cpp \
    sources/ast_buildin_function_call.cpp \
    sources/ast_cell_access_node.cpp \
    sources/ast_else_node.cpp \
    sources/ast_function_implementation_node.cpp \
    sources/ast_node.cpp \
    sources/ast_operator_node.cpp \
    sources/ast_variable.cpp \
    sources/ast_if_node.cpp \
    sources/parser.cpp \
    sources/syntax_error_handler.cpp \
    main.cpp \
    sources/tokenizer.cpp
