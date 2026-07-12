QT       += core gui widgets

CONFIG += c++17

TARGET = SistemaEventos
TEMPLATE = app

# Carpetas donde Qt buscará los archivos de cabecera (.h)
INCLUDEPATH += include

# Tus archivos de código fuente (.cpp)
SOURCES += \
    src/main.cpp \
    src/eventos.cpp \
    src/archivos.cpp \
    src/validaciones.cpp

# Tus archivos de cabecera (.h)
HEADERS += \
    include/evento.h \
    include/organizador.h \
    include/eventos.h \
    include/archivos.h \
    include/validaciones.h

# Tus interfaces gráficas diseñadas (.ui)
FORMS += \
    ui/registrarEvento.ui \
    ui/editarEvento.ui
