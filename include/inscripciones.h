#ifndef INSCRIPCIONES_H
#define INSCRIPCIONES_H

#include <string>
#include "inscripcion.h"
#include "participante.h"

using namespace std;

bool inscribirParticipante(const string &codigoAlumno, int idEvento);

bool anularInscripcion(int idInscripcion);

bool buscarInscripcion(int idInscripcion, Inscripcion &inscripcionEncontrada);

bool actualizarEstado(int idInscripcion, bool nuevaAsistencia);

#endif // INSCRIPCIONES_H
