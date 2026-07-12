#ifndef EVENTOS_H
#define EVENTOS_H

#include "evento.h"

int registrarEvento(Evento nuevoEvento);
bool editarEvento(int idEvento, Evento eventoModificado);
bool eliminarEvento(int idEvento);
Evento buscarEvento(int idEvento);
void listarEventos();

#endif
