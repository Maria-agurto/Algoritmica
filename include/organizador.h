#ifndef ORGANIZADOR_H
#define ORGANIZADOR_H

const int NOMBRE_MAX=81;
const int DEPARTAMENTO_MAX=61;

struct Organizador{
	int idOrganizador;
	char nombre[NOMBRE_MAX];
	char departamento[DEPARTAMENTO_MAX];
};

#endif
