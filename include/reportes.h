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

void ordenarEventos(int criterio, bool ascendente);
void filtrarEventos(int criterio, const string &valor);

#endif
