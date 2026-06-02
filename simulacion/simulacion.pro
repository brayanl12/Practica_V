QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Archivos fuente
SOURCES += \
    main.cpp \
    widget.cpp \
    Particula.cpp \
    Obstaculo.cpp \
    Simulacion.cpp

# Archivos de cabecera
HEADERS += \
    widget.h \
    Particula.h \
    Obstaculo.h \
    Simulacion.h

# Formulario de Qt Designer
FORMS += \
    widget.ui

# Regla por defecto para despliegue en Android
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
