#include "reportes.h"
#include "validaciones.h"
#include "utilidades.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

static const string AEVENTOS = "data/eventos.dat";
static const string APARTICIPANTES = "data/participantes.dat";
static const string AINSCRIPCIONES = "data/inscripciones.dat";

//Funciones internas
// Formato asumido de cada linea (separado por '|'):
//   eventos.dat        -> idEvento|titulo|categoria|fecha|horaInicio|horaFin|capacidad|inscritos|estado|organizador
//   participantes.dat  -> codigo|nombres|apellidos|carrera|facultad
//   inscripciones.dat  -> idInscripcion|idEvento|codigoAlumno|fechaRegistro|asistencia(0/1)
// ---------------------------------------------------------------------

// Divide una linea de texto en un vector de campos usando '|' como separador
static vector<string> dividirLinea(const string &linea, char separador = '|') {
    vector<string> campos;
    stringstream ss(linea);
    string campo;
    while (getline(ss, campo, separador)) {
        campos.push_back(trim(campo)); // se usa trim() de utilidades.h
    }
    return campos;
}

// Carga todos los eventos registrados en eventos.dat
static vector<Evento> cargarEventos() {
    vector<Evento> eventos;
    ifstream archivo(AEVENTOS);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de eventos (" << AEVENTOS << ")." << endl;
        return eventos;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        vector<string> campos = dividirLinea(linea);
        if (campos.size() < 10) continue; // linea corrupta o incompleta, se ignora

        Evento e;
        e.idEvento    = stoi(campos[0]);
        e.titulo      = campos[1];
        e.categoria   = campos[2];
        e.fecha       = campos[3];
        e.horaInicio  = campos[4];
        e.horaFin     = campos[5];
        e.capacidad   = stoi(campos[6]);
        e.inscritos   = stoi(campos[7]);
        e.estado      = campos[8];
        e.organizador = campos[9];

        eventos.push_back(e);
    }

    archivo.close();
    return eventos;
}

// Carga todos los participantes registrados en participantes.dat
static vector<Participante> cargarParticipantes() {
    vector<Participante> participantes;
    ifstream archivo(APARTICIPANTES);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de participantes (" << APARTICIPANTES << ")." << endl;
        return participantes;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        vector<string> campos = dividirLinea(linea);
        if (campos.size() < 5) continue;

        Participante p;
        p.codigo    = campos[0];
        p.nombres   = campos[1];
        p.apellidos = campos[2];
        p.carrera   = campos[3];
        p.facultad  = campos[4];

        participantes.push_back(p);
    }

    archivo.close();
    return participantes;
}

// Carga todas las inscripciones registradas en inscripciones.dat
static vector<Inscripcion> cargarInscripciones() {
    vector<Inscripcion> inscripciones;
    ifstream archivo(AINSCRIPCIONES);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de inscripciones (" << AINSCRIPCIONES << ")." << endl;
        return inscripciones;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        vector<string> campos = dividirLinea(linea);
        if (campos.size() < 5) continue;

        Inscripcion i;
        i.idInscripcion = stoi(campos[0]);
        i.idEvento      = stoi(campos[1]);
        i.codigoAlumno  = campos[2];
        i.fechaRegistro = campos[3];
        i.asistencia    = (campos[4] == "1");

        inscripciones.push_back(i);
    }

    archivo.close();
    return inscripciones;
}

// Imprime el encabezado de una tabla de eventos (evita repetir codigo)
static void imprimirEncabezadoEventos() {
    cout << left
         << setw(5)  << "ID"
         << setw(30) << "Titulo"
         << setw(15) << "Categoria"
         << setw(12) << "Fecha"
         << setw(10) << "Capacidad"
         << setw(10) << "Inscritos"
         << setw(12) << "Estado" << endl;
    cout << string(94, '-') << endl;
}

// Imprime una fila de la tabla de eventos
static void imprimirFilaEvento(const Evento &e) {
    cout << left
         << setw(5)  << e.idEvento
         << setw(30) << e.titulo
         << setw(15) << e.categoria
         << setw(12) << e.fecha
         << setw(10) << e.capacidad
         << setw(10) << e.inscritos
         << setw(12) << e.estado << endl;
}

// =======================================================================
// 1. generarReporteGeneral()
// Genera estadisticas del sistema: total de eventos, participantes,
// inscripciones y un resumen de eventos agrupados por estado.
// =======================================================================
void generarReporteGeneral() {
    vector<Evento> eventos = cargarEventos();
    vector<Participante> participantes = cargarParticipantes();
    vector<Inscripcion> inscripciones = cargarInscripciones();

    int totalDisponibles = 0, totalLlenos = 0, totalFinalizados = 0;
    int totalCapacidad = 0, totalInscritosSistema = 0;

    for (const Evento &e : eventos) {
        totalCapacidad += e.capacidad;
        totalInscritosSistema += e.inscritos;

        if (e.estado == "Disponible") totalDisponibles++;
        else if (e.estado == "Lleno") totalLlenos++;
        else if (e.estado == "Finalizado") totalFinalizados++;
    }

    // Se usa obtenerFechaActual() de utilidades.h para dejar constancia
    // de cuando se genero el reporte
    cout << "==================== REPORTE GENERAL DEL SISTEMA ====================" << endl;
    cout << "Fecha del reporte: " << obtenerFechaActual() << endl << endl;

    cout << "Total de eventos registrados:        " << eventos.size() << endl;
    cout << "  - Disponibles:                     " << totalDisponibles << endl;
    cout << "  - Llenos:                          " << totalLlenos << endl;
    cout << "  - Finalizados:                     " << totalFinalizados << endl;
    cout << endl;

    cout << "Total de participantes registrados:  " << participantes.size() << endl;
    cout << "Total de inscripciones registradas:  " << inscripciones.size() << endl;
    cout << endl;

    cout << "Capacidad total ofertada:            " << totalCapacidad << endl;
    cout << "Total de cupos ocupados:             " << totalInscritosSistema << endl;

    if (totalCapacidad > 0) {
        double ocupacionGlobal = (static_cast<double>(totalInscritosSistema) / totalCapacidad) * 100.0;
        cout << "Ocupacion global del sistema:        "
             << fixed << setprecision(2) << ocupacionGlobal << "%" << endl;
    }

    cout << "=======================================================================" << endl;
}

// =======================================================================
// 2. eventoMayorAsistencia()
// Recorre todos los eventos y muestra el que tiene mayor numero de
// inscritos (mayor asistencia registrada).
// =======================================================================
void eventoMayorAsistencia() {
    vector<Evento> eventos = cargarEventos();

    if (eventos.empty()) {
        cout << "No hay eventos registrados en el sistema." << endl;
        return;
    }

    Evento mayor = eventos[0];
    for (const Evento &e : eventos) {
        if (e.inscritos > mayor.inscritos) {
            mayor = e;
        }
    }

    cout << "===== EVENTO CON MAYOR ASISTENCIA =====" << endl;
    imprimirEncabezadoEventos();
    imprimirFilaEvento(mayor);
    cout << "Organizador: " << mayor.organizador << endl;
    cout << "========================================" << endl;
}

// =======================================================================
// 3. eventosDisponibles()
// Lista todos los eventos cuyo estado es "Disponible".
// =======================================================================
void eventosDisponibles() {
    vector<Evento> eventos = cargarEventos();

    cout << "===== EVENTOS DISPONIBLES =====" << endl;
    imprimirEncabezadoEventos();

    int contador = 0;
    for (const Evento &e : eventos) {
        if (e.estado == "Disponible") {
            imprimirFilaEvento(e);
            contador++;
        }
    }

    if (contador == 0) {
        cout << "No hay eventos con estado 'Disponible' en este momento." << endl;
    }
}

// =======================================================================
// 4. eventosLlenos()
// Lista todos los eventos cuyo estado es "Lleno".
// =======================================================================
void eventosLlenos() {
    vector<Evento> eventos = cargarEventos();

    cout << "===== EVENTOS LLENOS =====" << endl;
    imprimirEncabezadoEventos();

    int contador = 0;
    for (const Evento &e : eventos) {
        if (e.estado == "Lleno") {
            imprimirFilaEvento(e);
            contador++;
        }
    }

    if (contador == 0) {
        cout << "No hay eventos con estado 'Lleno' en este momento." << endl;
    }
}

// =======================================================================
// 5. porcentajeOcupacion()
// Calcula y muestra el porcentaje de ocupacion (inscritos / capacidad)
// de cada evento registrado.
// =======================================================================
void porcentajeOcupacion() {
    vector<Evento> eventos = cargarEventos();

    if (eventos.empty()) {
        cout << "No hay eventos registrados en el sistema." << endl;
        return;
    }

    cout << "===== PORCENTAJE DE OCUPACION POR EVENTO =====" << endl;
    cout << left << setw(5) << "ID" << setw(30) << "Titulo"
         << setw(12) << "Ocupacion" << endl;
    cout << string(50, '-') << endl;

    for (const Evento &e : eventos) {
        double porcentaje = 0.0;
        if (e.capacidad > 0) {
            porcentaje = (static_cast<double>(e.inscritos) / e.capacidad) * 100.0;
        }

        cout << left << setw(5) << e.idEvento << setw(30) << e.titulo
             << fixed << setprecision(2) << porcentaje << "%" << endl;
    }
}

// =======================================================================
// 6. ordenarEventos()
// Ordena la lista de eventos segun el criterio elegido por el usuario
// (titulo, fecha, capacidad o inscritos) en orden ascendente o
// descendente, y muestra el resultado.
// =======================================================================
void ordenarEventos() {
    vector<Evento> eventos = cargarEventos();

    if (eventos.empty()) {
        cout << "No hay eventos registrados en el sistema." << endl;
        return;
    }

    cout << "===== ORDENAR EVENTOS =====" << endl;
    cout << "Seleccione el criterio de ordenamiento:" << endl;
    cout << "1. Titulo" << endl;
    cout << "2. Fecha" << endl;
    cout << "3. Capacidad" << endl;
    cout << "4. Inscritos" << endl;
    cout << "Opcion: ";

    string opcion;
    getline(cin, opcion);

    // Se reutiliza validarNumero() de validaciones.h para asegurarse de
    // que el usuario ingreso un numero valido
    if (!validarNumero(opcion)) {
        cout << "Opcion invalida. Debe ingresar un numero." << endl;
        return;
    }

    cout << "Orden: 1. Ascendente   2. Descendente" << endl;
    cout << "Opcion: ";
    string ordenStr;
    getline(cin, ordenStr);

    if (!validarNumero(ordenStr)) {
        cout << "Opcion de orden invalida." << endl;
        return;
    }

    int criterio = stoi(opcion);
    bool ascendente = (stoi(ordenStr) == 1);

    sort(eventos.begin(), eventos.end(), [criterio, ascendente](const Evento &a, const Evento &b) {
        bool resultado;
        switch (criterio) {
            case 1: resultado = a.titulo < b.titulo; break;
            case 2: resultado = a.fecha < b.fecha; break;
            case 3: resultado = a.capacidad < b.capacidad; break;
            case 4: resultado = a.inscritos < b.inscritos; break;
            default: resultado = a.idEvento < b.idEvento; break;
        }
        return ascendente ? resultado : !resultado;
    });

    imprimirEncabezadoEventos();
    for (const Evento &e : eventos) {
        imprimirFilaEvento(e);
    }
}

// =======================================================================
// 7. filtrarEventos()
// Permite filtrar eventos por categoria, estado o fecha, segun lo que
// el usuario indique. Reutiliza validaciones.h para verificar que los
// campos ingresados no esten vacios y utilidades.h para normalizar el
// texto antes de comparar (comparacion sin distinguir mayusculas).
// =======================================================================
void filtrarEventos() {
    vector<Evento> eventos = cargarEventos();

    if (eventos.empty()) {
        cout << "No hay eventos registrados en el sistema." << endl;
        return;
    }

    cout << "===== FILTRAR EVENTOS =====" << endl;
    cout << "Filtrar por:" << endl;
    cout << "1. Categoria" << endl;
    cout << "2. Estado" << endl;
    cout << "3. Fecha" << endl;
    cout << "Opcion: ";

    string opcion;
    getline(cin, opcion);

    if (!validarNumero(opcion)) {
        cout << "Opcion invalida." << endl;
        return;
    }

    int criterio = stoi(opcion);
    cout << "Ingrese el valor a buscar: ";
    string valor;
    getline(cin, valor);

    // Se usa validarCampoVacio() de validaciones.h para no procesar
    // busquedas vacias
    if (!validarCampoVacio(valor)) {
        cout << "El valor de busqueda no puede estar vacio." << endl;
        return;
    }

    // Si el filtro es por fecha, se valida el formato dd/mm/yyyy
    if (criterio == 3 && !validarFecha(valor)) {
        cout << "La fecha ingresada no tiene un formato valido (dd/mm/yyyy)." << endl;
        return;
    }

    // Se normaliza el valor de busqueda a mayusculas para comparar sin
    // distinguir entre mayusculas y minusculas
    string valorNormalizado = convertirMayusculas(trim(valor));

    imprimirEncabezadoEventos();
    int coincidencias = 0;

    for (const Evento &e : eventos) {
        bool coincide = false;

        switch (criterio) {
            case 1: coincide = (convertirMayusculas(e.categoria) == valorNormalizado); break;
            case 2: coincide = (convertirMayusculas(e.estado) == valorNormalizado); break;
            case 3: coincide = (e.fecha == valor); break;
            default: coincide = false; break;
        }

        if (coincide) {
            imprimirFilaEvento(e);
            coincidencias++;
        }
    }

    if (coincidencias == 0) {
        cout << "No se encontraron eventos que coincidan con el filtro." << endl;
    }
}
