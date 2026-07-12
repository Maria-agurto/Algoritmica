QT       += core gui widgets

CONFIG += c++17

TARGET = SistemaEventos
TEMPLATE = app

# Carpetas donde Qt buscará los archivos de cabecera (.h)
INCLUDEPATH += include

# Archivos de código fuente (.cpp) de todo el equipo
SOURCES += \
    src/main.cpp \
    src/eventos.cpp \
    src/archivos.cpp \
    src/validaciones.cpp \
    src/utilidades.cpp \
    src/participantes.cpp \
    src/inscripciones.cpp \
    src/reportes.cpp \
    src/registrareventowindow.cpp \
    src/editareventowindow.cpp \
    src/registrarparticipantewindow.cpp \
    src/registrarorganizadorwindow.cpp \
    src/inscripcionwindow.cpp \
    src/reporteswindow.cpp \
    src/mainwindow.cpp \
    src/splashwindow.cpp \
    src/acercadewindow.cpp

# Archivos de cabecera (.h) de todo el equipo
HEADERS += \
    include/evento.h \
    include/organizador.h \
    include/participante.h \
    include/inscripcion.h \
    include/eventos.h \
    include/archivos.h \
    include/validaciones.h \
    include/utilidades.h \
    include/participantes.h \
    include/inscripciones.h \
    include/reportes.h \
    include/registrareventowindow.h \
    include/editareventowindow.h \
    include/registrarparticipantewindow.h \
    include/registrarorganizadorwindow.h \
    include/inscripcionwindow.h \
    include/reporteswindow.h \
    include/mainwindow.h \
    include/splashwindow.h \
    include/acercadewindow.h

# Interfaces gráficas diseñadas (.ui) de todo el equipo
FORMS += \
    ui/registrarEvento.ui \
    ui/editarEvento.ui \
    ui/registrarParticipante.ui \
    ui/registrarOrganizador.ui \
    ui/Inscripcion.ui \
    ui/reportes.ui \
    ui/mainwindow.ui \
    ui/splash.ui \
    ui/acercaDe.ui

# Recursos (estilos, iconos, imagenes)
RESOURCES += \
    resources/recursos.qrc
