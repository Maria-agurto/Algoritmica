#include "inscripciones.h"
#include "inscripcion.h"
#include "participante.h"

#include <fstream>
#include <vector>
#include <string>

//====================================================
// NOTA DE INTEGRACION
//
// Las siguientes cabeceras pertenecen a otros modulos
// del sistema y se asumen disponibles al momento de la
// integracion final del proyecto:
//
//   - validaciones.h  (Maria / equipo) -> validarCampoVacio(), validarNumero()
//   - utilidades.h    (Diego)          -> obtenerFechaActual()
//
// Mientras esos archivos no estan disponibles, este
// modulo puede compilarse de forma independiente
// comentando estas dos lineas y usando las versiones
// temporales marcadas como TODO mas abajo.
//====================================================
#include "validaciones.h"
#include "utilidades.h"

//----------------------------------------------------
// Nombres de los archivos de datos usados por este
// modulo. Se definen como constantes para evitar
// "cadenas magicas" repetidas en el codigo.
//----------------------------------------------------
static const std::string ARCHIVO_PARTICIPANTES = "data/participantes.dat";
static const std::string ARCHIVO_INSCRIPCIONES = "data/inscripciones.dat";

//====================================================
// SECCION: Serializacion binaria
//
// std::string no puede escribirse directamente con
// fwrite/write porque internamente maneja memoria
// dinamica. Por eso, cada string se guarda como:
// [tamano (size_t)][contenido de la cadena]
//====================================================

//----------------------------------------------------
// Escribe un std::string en un archivo binario.
//----------------------------------------------------
static void escribirString(std::ofstream &archivo, const std::string &texto)
{
    size_t tamano = texto.size();
    archivo.write(reinterpret_cast<const char *>(&tamano), sizeof(tamano));
    archivo.write(texto.c_str(), static_cast<std::streamsize>(tamano));
}

//----------------------------------------------------
// Lee un std::string desde un archivo binario.
//----------------------------------------------------
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

//----------------------------------------------------
// Escribe un registro Inscripcion completo en el
// archivo binario.
//----------------------------------------------------
static void escribirInscripcion(std::ofstream &archivo, const Inscripcion &insc)
{
    archivo.write(reinterpret_cast<const char *>(&insc.idInscripcion), sizeof(insc.idInscripcion));
    archivo.write(reinterpret_cast<const char *>(&insc.idEvento), sizeof(insc.idEvento));
    escribirString(archivo, insc.codigoAlumno);
    escribirString(archivo, insc.fechaRegistro);
    archivo.write(reinterpret_cast<const char *>(&insc.asistencia), sizeof(insc.asistencia));
}

//----------------------------------------------------
// Lee un registro Inscripcion completo desde el
// archivo binario. Retorna false si ya no hay mas
// registros por leer (fin de archivo).
//----------------------------------------------------
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

//----------------------------------------------------
// Lee un registro Participante completo desde el
// archivo binario. Retorna false si ya no hay mas
// registros por leer.
//
// Nota: este modulo solo LEE participantes (para
// validar su existencia). El alta de participantes
// no forma parte del alcance del Modulo 2.
//----------------------------------------------------
static bool leerParticipante(std::ifstream &archivo, Participante &part)
{
    leerString(archivo, part.codigo);
    if (archivo.eof())
    {
        return false;
    }
    leerString(archivo, part.nombres);
    leerString(archivo, part.apellidos);
    leerString(archivo, part.carrera);
    leerString(archivo, part.facultad);
    return true;
}

//====================================================
// SECCION: Funciones auxiliares privadas
//====================================================

//----------------------------------------------------
// Verifica si existe un participante con el codigo
// indicado dentro de participantes.dat
//----------------------------------------------------
static bool participanteExiste(const std::string &codigoAlumno)
{
    std::ifstream archivo(ARCHIVO_PARTICIPANTES, std::ios::binary);
    if (!archivo.is_open())
    {
        return false;
    }

    Participante actual;
    while (leerParticipante(archivo, actual))
    {
        if (actual.codigo == codigoAlumno)
        {
            return true;
        }
    }
    return false;
}

//----------------------------------------------------
// Estructura minima usada unicamente para LEER los
// campos de eventos.dat necesarios para validar cupo.
//
// IMPORTANTE (integracion): esta estructura debe
// coincidir con el orden y tipos en que el modulo de
// Eventos (Maria) escribe eventos.dat. Debe
// confirmarse con ella al integrar los modulos.
//----------------------------------------------------
struct EventoLectura
{
    int idEvento;
    std::string titulo;
    std::string categoria;
    std::string fecha;
    std::string horaInicio;
    std::string horaFin;
    int capacidad;
    int inscritos;
    std::string estado;
    std::string organizador;
};

static bool leerEvento(std::ifstream &archivo, EventoLectura &ev)
{
    archivo.read(reinterpret_cast<char *>(&ev.idEvento), sizeof(ev.idEvento));
    if (archivo.eof())
    {
        return false;
    }
    leerString(archivo, ev.titulo);
    leerString(archivo, ev.categoria);
    leerString(archivo, ev.fecha);
    leerString(archivo, ev.horaInicio);
    leerString(archivo, ev.horaFin);
    archivo.read(reinterpret_cast<char *>(&ev.capacidad), sizeof(ev.capacidad));
    archivo.read(reinterpret_cast<char *>(&ev.inscritos), sizeof(ev.inscritos));
    leerString(archivo, ev.estado);
    leerString(archivo, ev.organizador);
    return true;
}

//----------------------------------------------------
// Verifica que el evento exista y tenga cupo
// disponible (inscritos < capacidad).
//
// Parametros de salida:
//   existe    - true si el evento fue encontrado.
//   tieneCupo - true si inscritos < capacidad.
//----------------------------------------------------
static void validarEventoYCupo(int idEvento, bool &existe, bool &tieneCupo)
{
    existe = false;
    tieneCupo = false;

    std::ifstream archivo("data/eventos.dat", std::ios::binary);
    if (!archivo.is_open())
    {
        return;
    }

    EventoLectura actual;
    while (leerEvento(archivo, actual))
    {
        if (actual.idEvento == idEvento)
        {
            existe = true;
            tieneCupo = (actual.inscritos < actual.capacidad);
            return;
        }
    }
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

//----------------------------------------------------
// Genera un nuevo idInscripcion autoincremental,
// buscando el mayor ID existente en inscripciones.dat
// y sumando 1. Si el archivo esta vacio, retorna 1.
//----------------------------------------------------
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

//----------------------------------------------------
// Carga todos los registros de inscripciones.dat en
// un vector, para poder reescribir el archivo tras
// una modificacion o eliminacion.
//----------------------------------------------------
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

//----------------------------------------------------
// Reescribe por completo inscripciones.dat con el
// contenido del vector recibido.
//----------------------------------------------------
static void guardarTodasLasInscripciones(const std::vector<Inscripcion> &lista)
{
    std::ofstream archivo(ARCHIVO_INSCRIPCIONES, std::ios::binary | std::ios::trunc);
    for (const Inscripcion &insc : lista)
    {
        escribirInscripcion(archivo, insc);
    }
}

//====================================================
// SECCION: Funciones principales del modulo
//====================================================

//====================================================
// Registra una nueva inscripcion en el sistema.
// Verifica que exista el participante y el evento,
// que el evento tenga cupo, que el participante no
// esta inscrito previamente, y que los datos sean
// validos antes de guardar el registro.
//====================================================
bool inscribirParticipante(const std::string &codigoAlumno, int idEvento)
{
    // 1. Validar que el codigo del alumno no esta vacio.
    if (!validarCampoVacio(codigoAlumno))
    {
        return false;
    }

    // 2. Validar que el ID de evento sea un valor valido.
    if (idEvento <= 0)
    {
        return false;
    }

    // 3. Verificar que el participante exista.
    if (!participanteExiste(codigoAlumno))
    {
        return false;
    }

    // 4. Verificar que el evento exista y tenga cupo.
    bool eventoExiste = false;
    bool eventoTieneCupo = false;
    validarEventoYCupo(idEvento, eventoExiste, eventoTieneCupo);

    if (!eventoExiste || !eventoTieneCupo)
    {
        return false;
    }

    // 5. Verificar que el participante no esta ya inscrito
    //    en ese mismo evento.
    if (yaInscrito(codigoAlumno, idEvento))
    {
        return false;
    }

    // 6. Construir el nuevo registro de inscripcion.
    Inscripcion nuevaInscripcion;
    nuevaInscripcion.idInscripcion = generarNuevoIdInscripcion();
    nuevaInscripcion.idEvento = idEvento;
    nuevaInscripcion.codigoAlumno = codigoAlumno;
    nuevaInscripcion.fechaRegistro = obtenerFechaActual();
    nuevaInscripcion.asistencia = false;

    // 7. Guardar el registro al final del archivo.
    std::ofstream archivo(ARCHIVO_INSCRIPCIONES, std::ios::binary | std::ios::app);
    if (!archivo.is_open())
    {
        return false;
    }
    escribirInscripcion(archivo, nuevaInscripcion);

    return true;
}

//====================================================
// Anula (elimina) una inscripcion existente,
// identificada por su idInscripcion.
//====================================================
bool anularInscripcion(int idInscripcion)
{
    std::vector<Inscripcion> lista = cargarTodasLasInscripciones();

    bool encontrada = false;
    std::vector<Inscripcion> listaActualizada;

    for (const Inscripcion &insc : lista)
    {
        if (insc.idInscripcion == idInscripcion)
        {
            encontrada = true;
            continue; // No se copia al nuevo listado: se elimina.
        }
        listaActualizada.push_back(insc);
    }

    if (!encontrada)
    {
        return false;
    }

    guardarTodasLasInscripciones(listaActualizada);
    return true;
}

//====================================================
// Busca una inscripcion por su idInscripcion dentro
// de inscripciones.dat.
//====================================================
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

//====================================================
// Actualiza el campo asistencia de una inscripcion
// existente.
//====================================================
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

    if (!encontrada)
    {
        return false;
    }

    guardarTodasLasInscripciones(lista);
    return true;
}
