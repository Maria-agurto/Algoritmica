#ifndef REPORTES_H
#define REPORTES_H

#include "evento.h"
#include "inscripcion.h"
#include "participante.h"

#include <iostream>
#include <string>

using namespace std;

void generarReporteGeneral();
void eventoMayorAsistencia();
void eventosDisponibles();
void eventosLlenos();
void porcentajeOcupacion();

//====================================================
// CORRECCION (integracion con reporteswindow.cpp):
// Antes estas dos funciones no recibian parametros y
// pedian el criterio/valor con getline(cin,...), lo cual
// es imposible de usar desde una app grafica (Qt no
// tiene "consola"). Ahora reciben los valores elegidos
// en los QComboBox/QLineEdit de reportes.ui.
//
// ordenarEventos:
//   criterio   1=Titulo 2=Fecha 3=Capacidad 4=Inscritos
//   ascendente true=ascendente false=descendente
//
// filtrarEventos:
//   criterio 1=Categoria 2=Estado 3=Fecha
//   valor    texto a buscar (para fecha, formato dd/mm/yyyy)
//====================================================
void ordenarEventos(int criterio, bool ascendente);
void filtrarEventos(int criterio, const string &valor);

#endif
