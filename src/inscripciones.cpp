#include "../include/inscripciones.h"
#include "../include/inscripcion.h"
#include "../include/participantes.h"
#include "../include/archivos.h"
#include "../include/evento.h"

#include <fstream>
#include <vector>
#include <string>
#include <cstring>

#include "../include/validaciones.h"
#include "../include/utilidades.h"

//----------------------------------------------------
// Nombres de los archivos de datos usados por este modulo.
//----------------------------------------------------
static const std::string ARCHIVO_INSCRIPCIONES = "data/inscripciones.dat";

//====================================================
// SECCION: Serializacion binaria de Inscripcion
//====================================================

static void escribirString(std::ofstream &archivo, const std::string &texto)
{
    size_t tamano = texto.size();
    archivo.write(reinterpret_cast<const char *>(&tamano), sizeof(tamano));
    archivo.write(texto.c_str(), static_cast<std::streamsize>(tamano));
}

static void leerString(std::ifstream &archivo, std::string &texto)
{
    size_t tamano = 0;
    archivo.read(reinterpret_cast<char *>(&tamano), sizeof(tamano));
    texto.resize(tamano);
    if (tamano > 0)
    {
        archivo.read(&texto[0], static_cast<std::streamsize>(tamano));
    }
}

static void escribirInscripcion(std::ofstream &archivo, const Inscripcion &insc)
{
    archivo.write(reinterpret_cast<const char *>(&insc.idInscripcion), sizeof(insc.idInscripcion));
    archivo.write(reinterpret_cast<const char *>(&insc.idEvento), sizeof(insc.idEvento));
    escribirString(archivo, insc.codigoAlumno);
    escribirString(archivo, insc.fechaRegistro);
    archivo.write(reinterpret_cast<const char *>(&insc.asistencia), sizeof(insc.asistencia));
}

static bool leerInscripcion(std::ifstream &archivo, Inscripcion &insc)
{
    archivo.read(reinterpret_cast<char *>(&insc.idInscripcion), sizeof(insc.idInscripcion));
    if (archivo.eof())
    {
        return false;
    }
    archivo.read(reinterpret_cast<char *>(&insc.idEvento), sizeof(insc.idEvento));
    leerString(archivo, insc.codigoAlumno);
    leerString(archivo, insc.fechaRegistro);
    archivo.read(reinterpret_cast<char *>(&insc.asistencia), sizeof(insc.asistencia));
    return true;
}

//====================================================
// SECCION: Funciones auxiliares privadas
//====================================================

//----------------------------------------------------
// Verifica si existe un participante con el codigo
//----------------------------------------------------
static bool participanteExiste(const std::string &codigoAlumno)
{
    Participante encontrado;
    return buscarParticipante(codigoAlumno, encontrado);
}

//----------------------------------------------------
//----------------------------------------------------
static void validarEventoYCupo(int idEvento, bool &existe, bool &tieneCupo)
{
    existe = false;
    tieneCupo = false;

    int cantidad = 0;
    Evento *eventos = leerTodosEventos(cantidad);
    if (eventos == NULL) return;

    for (int i = 0; i < cantidad; i++)
    {
        if (eventos[i].idEvento == idEvento)
        {
            existe = true;
            tieneCupo = (eventos[i].inscritos < eventos[i].capacidad);
            break;
        }
    }
    delete[] eventos;
}

//----------------------------------------------------
// Verifica si un participante ya esta inscrito en un
// evento especifico, recorriendo inscripciones.dat.
//----------------------------------------------------
static bool yaInscrito(const std::string &codigoAlumno, int idEvento)
{
    std::ifstream archivo(ARCHIVO_INSCRIPCIONES, std::ios::binary);
    if (!archivo.is_open())
    {
        return false;
    }

    Inscripcion actual;
    while (leerInscripcion(archivo, actual))
    {
        if (actual.codigoAlumno == codigoAlumno && actual.idEvento == idEvento)
        {
            return true;
        }
    }
    return false;
}

static int generarNuevoIdInscripcion()
{
    std::ifstream archivo(ARCHIVO_INSCRIPCIONES, std::ios::binary);
    int maximoId = 0;

    if (archivo.is_open())
    {
        Inscripcion actual;
        while (leerInscripcion(archivo, actual))
        {
            if (actual.idInscripcion > maximoId)
            {
                maximoId = actual.idInscripcion;
            }
        }
    }

    return maximoId + 1;
}

static std::vector<Inscripcion> cargarTodasLasInscripciones()
{
    std::vector<Inscripcion> lista;
    std::ifstream archivo(ARCHIVO_INSCRIPCIONES, std::ios::binary);

    if (archivo.is_open())
    {
        Inscripcion actual;
        while (leerInscripcion(archivo, actual))
        {
            lista.push_back(actual);
        }
    }
    return lista;
}

static void guardarTodasLasInscripciones(const std::vector<Inscripcion> &lista)
{
    std::ofstream archivo(ARCHIVO_INSCRIPCIONES, std::ios::binary | std::ios::trunc);
    for (const Inscripcion &insc : lista)
    {
        escribirInscripcion(archivo, insc);
    }
}

//----------------------------------------------------
//----------------------------------------------------
static bool incrementarInscritosEvento(int idEvento)
{
    int cantidad = 0;
    Evento *eventos = leerTodosEventos(cantidad);
    if (eventos == NULL) return false;

    bool encontrado = false;
    for (int i = 0; i < cantidad; i++)
    {
        if (eventos[i].idEvento == idEvento)
        {
            eventos[i].inscritos++;
            if (eventos[i].inscritos >= eventos[i].capacidad)
            {
                strncpy(eventos[i].estado, "Lleno", ESTADO_MAX - 1);
                eventos[i].estado[ESTADO_MAX - 1] = '\0';
            }
            encontrado = true;
            break;
        }
    }

    if (encontrado) sobreescribirEventos(eventos, cantidad);
    delete[] eventos;
    return encontrado;
}

//====================================================
// SECCION: Funciones principales del modulo
//====================================================

bool inscribirParticipante(const std::string &codigoAlumno, int idEvento)
{
    if (!validarCampoVacio(codigoAlumno))
    {
        return false;
    }

    if (idEvento <= 0)
    {
        return false;
    }

    if (!participanteExiste(codigoAlumno))
    {
        return false;
    }

    bool eventoExiste = false;
    bool eventoTieneCupo = false;
    validarEventoYCupo(idEvento, eventoExiste, eventoTieneCupo);

    if (!eventoExiste || !eventoTieneCupo)
    {
        return false;
    }

    if (yaInscrito(codigoAlumno, idEvento))
    {
        return false;
    }

    Inscripcion nuevaInscripcion;
    nuevaInscripcion.idInscripcion = generarNuevoIdInscripcion();
    nuevaInscripcion.idEvento = idEvento;
    nuevaInscripcion.codigoAlumno = codigoAlumno;
    nuevaInscripcion.fechaRegistro = obtenerFechaActual();
    nuevaInscripcion.asistencia = false;

    std::ofstream archivo(ARCHIVO_INSCRIPCIONES, std::ios::binary | std::ios::app);
    if (!archivo.is_open())
    {
        return false;
    }
    escribirInscripcion(archivo, nuevaInscripcion);
    archivo.close();

    // CORRECCION: actualizar el contador de cupo del evento.
    incrementarInscritosEvento(idEvento);

    return true;
}

bool anularInscripcion(int idInscripcion)
{
    std::vector<Inscripcion> lista = cargarTodasLasInscripciones();

    bool encontrada = false;
    std::vector<Inscripcion> listaActualizada;
    int idEventoAfectado = -1;

    for (const Inscripcion &insc : lista)
    {
        if (insc.idInscripcion == idInscripcion)
        {
            encontrada = true;
            idEventoAfectado = insc.idEvento;
            continue; // No se copia al nuevo listado: se elimina.
        }
        listaActualizada.push_back(insc);
    }

    if (!encontrada)
    {
        return false;
    }

    guardarTodasLasInscripciones(listaActualizada);

    // CORRECCION: al anular, se libera un cupo en el evento.
    if (idEventoAfectado != -1)
    {
        int cantidad = 0;
        Evento *eventos = leerTodosEventos(cantidad);
        if (eventos != NULL)
        {
            for (int i = 0; i < cantidad; i++)
            {
                if (eventos[i].idEvento == idEventoAfectado)
                {
                    if (eventos[i].inscritos > 0) eventos[i].inscritos--;
                    if (eventos[i].inscritos < eventos[i].capacidad)
                    {
                        strncpy(eventos[i].estado, "Disponible", ESTADO_MAX - 1);
                        eventos[i].estado[ESTADO_MAX - 1] = '\0';
                    }
                    break;
                }
            }
            sobreescribirEventos(eventos, cantidad);
            delete[] eventos;
        }
    }

    return true;
}

bool buscarInscripcion(int idInscripcion, Inscripcion &inscripcionEncontrada)
{
    std::ifstream archivo(ARCHIVO_INSCRIPCIONES, std::ios::binary);
    if (!archivo.is_open())
    {
        return false;
    }

    Inscripcion actual;
    while (leerInscripcion(archivo, actual))
    {
        if (actual.idInscripcion == idInscripcion)
        {
            inscripcionEncontrada = actual;
            return true;
        }
    }
    return false;
}

bool actualizarEstado(int idInscripcion, bool nuevaAsistencia)
{
    std::vector<Inscripcion> lista = cargarTodasLasInscripciones();

    bool encontrada = false;
    for (Inscripcion &insc : lista)
    {
        if (insc.idInscripcion == idInscripcion)
        {
            insc.asistencia = nuevaAsistencia;
            encontrada = true;
            break;
        }
    }

    if (!encontrada) return false;

    guardarTodasLasInscripciones(lista);
    return true;
}
