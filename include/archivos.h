#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <string>
#include "evento.h"
#include "organizador.h"
using namespace std;

const string ARCHIVO_EVENTOS="data/eventos.dat";
const string ARCHIVO_ORGANIZADORES="data/organizadores.dat";

bool escribirEvento(Evento evento);
Evento* leerTodosEventos(int &cantidad);
bool sobreescribirEventos(Evento *eventos, int cantidad);

bool escribirOrganizador(Organizador organizador);
Organizador* leerTodosOrganizadores(int &cantidad);
bool sobreescribirOrganizadores(Organizador *organizadores, int cantidad);

int generarID(string nombreArchivo);

#endif
