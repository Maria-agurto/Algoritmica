#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#include <string>
using namespace std;

bool validarTexto(string texto, int maxLen);
bool validarNumero(int numero);
bool validarNumero(const string &texto);
bool validarFecha(string fecha);
bool validarHora(string horaInicio, string horaFin);
bool validarCampoVacio(string texto);

#endif
