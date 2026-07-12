#ifndef EVENTO_H
#define EVENTO_H

const int TITULO_MAX=101;
const int CATEGORIA_MAX=30;
const int FECHA_MAX=11;
const int HORA_MAX=6;
const int ESTADO_MAX=15;

struct Evento{
	int idEvento;
	char titulo[TITULO_MAX];
	char categoria[CATEGORIA_MAX];
	char fecha[FECHA_MAX];
	char horaInicio[HORA_MAX];
	char horaFin[HORA_MAX];
	int capacidad;
	int inscritos;
	char estado[ESTADO_MAX];
	int idOrganizador;
};

#endif
