#ifndef INSCRIPCION_H
#define INSCRIPCION_H

#include <string>

using namespace std;
struct Inscripcion
{
    int idInscripcion;
    int idEvento;
    string codigoAlumno;
    string fechaRegistro;
    bool asistencia;
};

#endif 
