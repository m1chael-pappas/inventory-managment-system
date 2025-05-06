# inventory.pro
QT += core gui widgets

TARGET = inventory_management
TEMPLATE = app

CONFIG += c++11

SOURCES += src/main.cpp \
    src/Product.cpp \
    src/InventoryManager.cpp \
    src/MainWindow.cpp

HEADERS += includes/Product.h \
    includes/InventoryManager.h \
    includes/MainWindow.h