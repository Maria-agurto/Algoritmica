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
void eventosDisponibles(); //Lista de eventos con estado Disponible
void eventosLlenos(); //Lista de eventos con estado Lleno
void porcentajeOcupacion();
void ordenarEventos();
void filtrarEventos();

#endif // REPORTES_H
