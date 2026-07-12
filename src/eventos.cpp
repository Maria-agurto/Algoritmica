#include "../include/eventos.h"
#include "../include/archivos.h"
#include <iostream>
#include <cstring>
using namespace std;

int registrarEvento(Evento nuevoEvento){
	Evento eventoAGuardar=nuevoEvento;
	eventoAGuardar.idEvento=generarID(ARCHIVO_EVENTOS);
	eventoAGuardar.inscritos=0;
	bool ok=escribirEvento(eventoAGuardar);
	if(!ok){
		cout<<"\nError: el evento NO se pudo guardar en "<<ARCHIVO_EVENTOS;
		return -1;
	}
	cout<<"\nEvento registrado correctamente con ID: "<<eventoAGuardar.idEvento;
	return eventoAGuardar.idEvento;
}

bool editarEvento(int idEvento, Evento eventoModificado){
	int cantidad=0;
	Evento *eventos=leerTodosEventos(cantidad);
	bool encontrado=false;
	for(int i=0;i<cantidad;i++){
		if(eventos[i].idEvento==idEvento){
			Evento actualizado=eventoModificado;
			actualizado.idEvento=idEvento;
			eventos[i]=actualizado;
			encontrado=true;
			break;
		}
	}
	if(encontrado) sobreescribirEventos(eventos,cantidad);
	delete[] eventos;
	return encontrado;
}

bool eliminarEvento(int idEvento){
	int cantidad=0;
	Evento *eventos=leerTodosEventos(cantidad);
	bool encontrado=false;
	for(int i=0;i<cantidad;i++){
		if(eventos[i].idEvento==idEvento){
			strncpy(eventos[i].estado, "Cancelado", ESTADO_MAX-1);
			eventos[i].estado[ESTADO_MAX-1]='\0';
			encontrado=true;
			break;
		}
	}
	if(encontrado) sobreescribirEventos(eventos,cantidad);
	delete[] eventos;
	return encontrado;
}

Evento buscarEvento(int idEvento){
	int cantidad=0;
	Evento *eventos=leerTodosEventos(cantidad);
	Evento resultado={};
	resultado.idEvento=-1;
	for(int i=0;i<cantidad;i++){
		if(eventos[i].idEvento==idEvento){
			resultado=eventos[i];
			break;
		}
	}
	delete[] eventos;
	return resultado;
}

void listarEventos(){
	int cantidad=0;
	Evento *eventos=leerTodosEventos(cantidad);
	if(cantidad==0 || eventos==NULL){
		cout<<"\nNo hay eventos registrados.";
		return;
	}
	cout<<"\n--- Listado de Eventos ---\n";
	for(int i=0;i<cantidad;i++){
		cout<<"ID: "<<eventos[i].idEvento
			<<" | Titulo: "<<eventos[i].titulo
			<<" | Categoria: "<<eventos[i].categoria
			<<" | Fecha: "<<eventos[i].fecha
			<<" | Hora: "<<eventos[i].horaInicio<<" - "<<eventos[i].horaFin
			<<" | Cupos: "<<eventos[i].inscritos<<"/"<<eventos[i].capacidad
			<<" | Estado: "<<eventos[i].estado
			<<" | Organizador ID: "<<eventos[i].idOrganizador<<"\n";
	}
	cout<<"--------------------------\n";
	delete[] eventos;
}
