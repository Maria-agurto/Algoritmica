#include "../include/reportes.h"
#include "../include/validaciones.h"
#include "../include/utilidades.h"
#include "../include/archivos.h"
#include "../include/participantes.h"
#include "../include/inscripciones.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cstring>

using namespace std;

//====================================================
// CORRECCION CRITICA (integracion):
//
// La version anterior de este archivo definia su propio
// "cargarEventos()"/"cargarParticipantes()" leyendo texto
// separado por '|' con getline(), y asumia que Evento y
// Participante tenian campos std::string (e.titulo,
// e.organizador, etc). Eso NO coincide con:
//
//   1) evento.h real: los campos son char[] de tamano
//      fijo (titulo, categoria, fecha...) y el campo de
//      responsable se llama idOrganizador (int), no existe
//      "organizador" como string.
//   2) archivos.cpp (Maria) escribe eventos.dat en BINARIO
//      crudo (archivo.write((char*)&evento, sizeof(Evento))),
//      nunca como texto con '|'.
//
// Por estas dos razones el archivo original NO compilaba
// (asignaciones de string a char[], comparaciones de
// char[] con "Disponible" via == que siempre son falsas
// aunque compilen) y aunque compilara jamas hubiera leido
// un evento real. Se corrige reutilizando las funciones
// que YA existen y funcionan: leerTodosEventos() de
// archivos.h y leerTodosParticipantes() del nuevo modulo
// participantes.h.
//
// CORRECCION ADICIONAL (DRY): este archivo tenia su PROPIA
// tercera copia del parser binario de inscripciones.dat
// (leerStringR/leerInscripcionR), identica a la de
// inscripciones.cpp y a la que tenia inscripcionwindow.cpp.
// Ahora se usa la funcion publica listarInscripciones() de
// inscripciones.h, que es la unica fuente de verdad para ese
// formato binario.
//====================================================

static vector<Evento> cargarEventos() {
	vector<Evento> eventos;
	int cantidad=0;
	Evento *arreglo=leerTodosEventos(cantidad);
	if(arreglo!=NULL){
		for(int i=0;i<cantidad;i++) eventos.push_back(arreglo[i]);
		delete[] arreglo;
	}
	return eventos;
}

static vector<Participante> cargarParticipantes() {
	return leerTodosParticipantes();
}

static vector<Inscripcion> cargarInscripciones() {
	return listarInscripciones();
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
	vector<Evento> eventos = cargarEventos();
	vector<Participante> participantes = cargarParticipantes();
	vector<Inscripcion> inscripciones = cargarInscripciones();

	int totalDisponibles = 0, totalLlenos = 0, totalFinalizados = 0;
	int totalCapacidad = 0, totalInscritosSistema = 0;

	for (const Evento &e : eventos) {
		totalCapacidad += e.capacidad;
		totalInscritosSistema += e.inscritos;
		if (strcmp(e.estado, "Disponible") == 0) totalDisponibles++;
		else if (strcmp(e.estado, "Lleno") == 0) totalLlenos++;
		else if (strcmp(e.estado, "Finalizado") == 0) totalFinalizados++;
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
	// CORRECCION: evento.h no tiene campo "organizador"
	// (string), tiene "idOrganizador" (int). Se imprime el ID;
	// si se quiere el nombre hay que cruzarlo con
	// leerTodosOrganizadores() de archivos.h.
	cout << "ID Organizador: " << mayor.idOrganizador << endl;
	cout << "========================================" << endl;
}

//*******************************
void eventosDisponibles() {
	vector<Evento> eventos = cargarEventos();

	cout << "===== EVENTOS DISPONIBLES =====" << endl;
	imprimirEncabezadoEventos();

	int contador = 0;
	for (const Evento &e : eventos) {
		if (strcmp(e.estado, "Disponible") == 0) {
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
		if (strcmp(e.estado, "Lleno") == 0) {
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
// CORRECCION: ahora recibe criterio/ascendente como
// parametros (vienen de cmbCriterioOrden/cmbOrdenDireccion
// en reporteswindow.cpp) en vez de leerlos con cin.
void ordenarEventos(int criterio, bool ascendente) {
	vector<Evento> eventos = cargarEventos();

	if (eventos.empty()) {
		cout << "No hay eventos registrados en el sistema." << endl;
		return;
	}

	sort(eventos.begin(), eventos.end(), [criterio, ascendente](const Evento &a, const Evento &b) {
		bool resultado;
		switch (criterio) {
			// CORRECCION: comparacion de char[] con strcmp,
			// no con < directo (eso compararia punteros).
			case 1: resultado = strcmp(a.titulo, b.titulo) < 0; break;
			case 2: resultado = strcmp(a.fecha, b.fecha) < 0; break;
			case 3: resultado = a.capacidad < b.capacidad; break;
			case 4: resultado = a.inscritos < b.inscritos; break;
			default: resultado = a.idEvento < b.idEvento; break;
		}
		return ascendente ? resultado : !resultado;
	});

	cout << "===== EVENTOS ORDENADOS =====" << endl;
	imprimirEncabezadoEventos();
	for (const Evento &e : eventos) {
		imprimirFilaEvento(e);
	}
}

//*******************************
// CORRECCION: ahora recibe criterio/valor como parametros
// (vienen de cmbCriterioFiltro/txtValorFiltro en
// reporteswindow.cpp) en vez de leerlos con cin.
void filtrarEventos(int criterio, const string &valor) {
	vector<Evento> eventos = cargarEventos();

	if (eventos.empty()) {
		cout << "No hay eventos registrados en el sistema." << endl;
		return;
	}

	if (!validarCampoVacio(valor)) {
		cout << "El valor de busqueda no puede estar vacio." << endl;
		return;
	}

	if (criterio == 3 && !validarFecha(valor)) {
		cout << "La fecha ingresada no tiene un formato valido (dd/mm/yyyy)." << endl;
		return;
	}

	string valorNormalizado = convertirMayusculas(trim(valor));

	imprimirEncabezadoEventos();
	int coincidencias = 0;

	for (const Evento &e : eventos) {
		bool coincide = false;

		switch (criterio) {
			case 1: coincide = (convertirMayusculas(e.categoria) == valorNormalizado); break;
			case 2: coincide = (convertirMayusculas(e.estado) == valorNormalizado); break;
			case 3: coincide = (string(e.fecha) == valor); break;
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
