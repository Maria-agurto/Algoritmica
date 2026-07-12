#ifndef PARTICIPANTE_H
#define PARTICIPANTE_H

#include <string>

//====================================================
// Struct: Participante
//
// Representa a un estudiante que puede inscribirse
// a uno o varios eventos académicos.
//
// Campos (según diccionario de datos del proyecto):
// - codigo   : Código del alumno (8 dígitos).
// - nombres  : Nombres del participante (solo letras).
// - apellidos: Apellidos del participante (solo letras).
// - carrera  : Carrera del participante (selección de listado).
// - facultad : Facultad a la que pertenece (selección de listado).
//
// Nota: la validación de estos campos (formato, longitud,
// que solo contengan letras, etc.) se realiza en
// validaciones.cpp y NO dentro de este struct.
//====================================================
using namespace std;
struct Participante
{
    string codigo;
    string nombres;
    string apellidos;
    string carrera;
    string facultad;
};

#endif // PARTICIPANTE_H
