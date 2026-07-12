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
static vector<string> dividirLinea(const string &linea, char separador = '|') {
    vector<string> campos;
    stringstream ss(linea);
    string campo;
    while (getline(ss, campo, separador)) {
        campos.push_back(trim(campo));
    }
    return campos;
}

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
        if (campos.size() < 10) continue; //Para ignorar líneas incompletas

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

//Funciones principales
//*******************************
void generarReporteGeneral() { 
    //Genera total de eventos, participantes, inscripciones y resumen de eventos por estado.
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

//*******************************
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

//*******************************
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

//*******************************
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

//*******************************
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

//*******************************
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

//*******************************
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

    if (!validarCampoVacio(valor)) {
        cout << "El valor de busqueda no puede estar vacio." << endl;
        return;
    }

    if (criterio == 3 && !validarFecha(valor)) {
        cout << "La fecha ingresada no tiene un formato valido (dd/mm/yyyy)." << endl;
        return;
    }

    //Búsqueda no distingue mayúsculas y minúsculas
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
