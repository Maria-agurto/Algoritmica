#ifndef INSCRIPCION_H
#define INSCRIPCION_H

#include <string>

//====================================================
// Struct: Inscripcion
//
// Representa la relación entre un participante y un
// evento académico al que se ha inscrito.
//
// Campos (según diccionario de datos del proyecto):
// - idInscripcion : Identificador único, autogenerado.
// - idEvento      : ID del evento al que pertenece la
//                    inscripción. Debe existir previamente
//                    en eventos.dat.
// - codigoAlumno  : Código del participante inscrito. Debe
//                    existir previamente en participantes.dat.
// - fechaRegistro : Fecha en la que se realizó la inscripción
//                    (formato dd/mm/yyyy).
// - asistencia    : Indica si el participante asistió al
//                    evento. Se inicializa siempre en false.
//
// Nota: la validación de existencia de evento/participante
// se realiza en inscripciones.cpp antes de crear el registro,
// no dentro de este struct.
//====================================================
struct Inscripcion
{
    int idInscripcion;
    int idEvento;
    std::string codigoAlumno;
    std::string fechaRegistro;
    bool asistencia;
};

#endif // INSCRIPCION_H
