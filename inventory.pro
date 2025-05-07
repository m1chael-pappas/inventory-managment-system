# inventory.pro
QT += core gui widgets charts

TARGET = inventory_management
TEMPLATE = app

CONFIG += c++11

SOURCES += src/main.cpp \
    src/product.cpp \
    src/inventory_manager.cpp \
    src/main_window.cpp

HEADERS += includes/product.h \
    includes/inventory_manager.h \
    includes/main_window.h