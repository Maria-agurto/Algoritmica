#ifndef INSCRIPCIONES_H
#define INSCRIPCIONES_H

#include <string>
#include "inscripcion.h"
#include "participante.h"

//====================================================
// Módulo: Inscripciones
//
// Declara las funciones principales del módulo de
// inscripciones, encargado de registrar, anular,
// buscar y actualizar el estado de las inscripciones
// de los participantes a los eventos académicos.
//
// La implementación se encuentra en inscripciones.cpp
//====================================================

//----------------------------------------------------
// Registra una nueva inscripción en el sistema.
//
// Parámetros:
//   codigoAlumno - Código del participante a inscribir.
//   idEvento     - ID del evento al que se desea inscribir.
//
// Retorno:
//   true  si la inscripción se registró correctamente.
//   false si alguna validación falló (participante o
//         evento inexistente, sin cupo, ya inscrito, etc.)
//----------------------------------------------------
using namespace std;
bool inscribirParticipante(const string &codigoAlumno, int idEvento);

//----------------------------------------------------
// Anula (elimina) una inscripción existente.
//
// Parámetros:
//   idInscripcion - ID de la inscripción a anular.
//
// Retorno:
//   true  si la inscripción existía y fue eliminada.
//   false si no se encontró la inscripción.
//----------------------------------------------------
bool anularInscripcion(int idInscripcion);

//----------------------------------------------------
// Busca una inscripción por su ID.
//
// Parámetros:
//   idInscripcion       - ID de la inscripción a buscar.
//   inscripcionEncontrada - Referencia donde se copiarán
//                           los datos de la inscripción
//                           si es encontrada.
//
// Retorno:
//   true  si la inscripción fue encontrada.
//   false si no existe una inscripción con ese ID.
//----------------------------------------------------
bool buscarInscripcion(int idInscripcion, Inscripcion &inscripcionEncontrada);

//----------------------------------------------------
// Actualiza el estado de asistencia de una inscripción.
//
// Parámetros:
//   idInscripcion  - ID de la inscripción a actualizar.
//   nuevaAsistencia - Nuevo valor del campo asistencia.
//
// Retorno:
//   true  si la inscripción fue encontrada y actualizada.
//   false si no existe una inscripción con ese ID.
//----------------------------------------------------
bool actualizarEstado(int idInscripcion, bool nuevaAsistencia);

#endif // INSCRIPCIONES_H
