#ifndef PARTICIPANTES_H
#define PARTICIPANTES_H

#include <string>
#include <vector>
#include "participante.h"

bool registrarParticipante(const Participante &nuevo);
bool eliminarParticipante(const std::string &codigo);
bool buscarParticipante(const std::string &codigo, Participante &encontrado);
std::vector<Participante> leerTodosParticipantes();

#endif
