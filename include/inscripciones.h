#ifndef INSCRIPCIONES_H
#define INSCRIPCIONES_H

#include <string>
#include <vector>
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

//----------------------------------------------------
// CORRECCION (consistencia/DRY): este modulo no exponia
// una funcion para listar TODAS las inscripciones, asi que
// inscripcionwindow.cpp y reportes.cpp habian copiado y
// reimplementado por su cuenta la lectura binaria del
// archivo (leerInscripcionLocal / leerInscripcionR), cada
// uno con su propia copia identica del formato. Si este
// modulo cambiaba el formato de guardado, esas copias se
// rompian en silencio porque nadie las actualizaba junto
// con inscripciones.cpp. Se expone ahora una unica funcion
// publica que reutiliza la MISMA logica interna de lectura,
// y las otras dos ventanas/archivos se actualizan para
// usarla en vez de su propia copia.
//
// Retorno:
//   Lista con todas las inscripciones registradas (vacia
//   si no hay ninguna o el archivo no existe todavia).
//----------------------------------------------------
std::vector<Inscripcion> listarInscripciones();

#endif // INSCRIPCIONES_H
