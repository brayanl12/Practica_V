TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        gestorcolisiones.cpp \
        main.cpp \
        obstaculo.cpp \
        particula.cpp \
        simulacion.cpp \
        vector2d.cpp

HEADERS += \
    gestorcolisiones.h \
    obstaculo.h \
    particula.h \
    simulacion.h \
    vector2d.h
