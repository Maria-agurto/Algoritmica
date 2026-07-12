#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string>
using namespace std;

//====================================================
// Modulo: Utilidades
//
// Funciones auxiliares de proposito general usadas por
// varios modulos del sistema (Eventos, Inscripciones,
// Reportes). No contienen logica de negocio propia de
// ningun modulo; solo dan soporte (fechas, texto, codigos).
//
// La implementacion se encuentra en utilidades.cpp
//====================================================

//----------------------------------------------------
// Devuelve la fecha actual del sistema en formato
// dd/mm/yyyy (mismo formato que usa el resto del
// sistema, ej: Evento::fecha).
//----------------------------------------------------
string obtenerFechaActual();

//----------------------------------------------------
// Arma una fecha en formato dd/mm/yyyy a partir de sus
// componentes numericos, rellenando con ceros a la
// izquierda cuando corresponde (ej: dia=5 -> "05").
//
// Parametros:
//   dia, mes, anio - componentes de la fecha.
//
// Retorno:
//   string con la fecha formateada como dd/mm/yyyy.
//----------------------------------------------------
string formatearFecha(int dia, int mes, int anio);

//----------------------------------------------------
// Convierte todos los caracteres de un texto a
// mayusculas. Se usa, por ejemplo, en Reportes para
// comparar filtros sin distinguir mayusculas/minusculas.
//----------------------------------------------------
string convertirMayusculas(string texto);

//----------------------------------------------------
// Elimina espacios en blanco al inicio y al final de
// un texto (no modifica espacios internos).
//----------------------------------------------------
string trim(string texto);

//----------------------------------------------------
// Genera un codigo alfanumerico aleatorio de la
// longitud indicada. Funcion generica de utilidad
// (por ejemplo, para codigos temporales o de
// respaldo); no reemplaza al codigo de 8 digitos del
// Participante, que se asigna segun las reglas propias
// del modulo de Inscripciones.
//
// Parametros:
//   longitud - cantidad de caracteres del codigo
//              generado (por defecto 8).
//----------------------------------------------------
string generarCodigo(int longitud=8);

#endif
