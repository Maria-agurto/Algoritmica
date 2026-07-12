#ifndef REPORTES_H
#define REPORTES_H

#include "evento.h"
#include "inscripcion.h"
#include "participante.h"

#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

void generarReporteGeneral();
void eventoMayorAsistencia();
void eventosDisponibles();
void eventosLlenos();
void porcentajeOcupacion();
void ordenarEventos();
void filtrarEventos();


vector<Evento> obtenerTodosLosEventos();
vector<Evento> obtenerEventosDisponiblesData();
vector<Evento> obtenerEventosLlenosData();
Evento obtenerEventoMayorAsistenciaData();
string obtenerReporteGeneralTexto();
vector<pair<Evento, double>> obtenerPorcentajeOcupacionData();
vector<Evento> obtenerEventosOrdenadosData(int criterio, bool ascendente);
vector<Evento> obtenerEventosFiltradosData(int criterio, const string &valor);

#endif
