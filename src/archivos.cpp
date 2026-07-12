#include "../include/archivos.h"
#include <fstream>
#include <iostream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

bool escribirEvento(Evento evento){
	fs::path ruta(ARCHIVO_EVENTOS);
	if(ruta.has_parent_path()) fs::create_directories(ruta.parent_path());
	ofstream archivo(ARCHIVO_EVENTOS, ios::binary|ios::app);
	if(!archivo){
		cout<<"\nError: no se pudo abrir "<<ARCHIVO_EVENTOS;
		return false;
	}
	archivo.write((char*)&evento, sizeof(Evento));
	archivo.close();
	return true;
}

Evento* leerTodosEventos(int &cantidad){
	cantidad=0;
	ifstream archivo(ARCHIVO_EVENTOS, ios::binary);
	if(!archivo) return NULL;
	archivo.seekg(0, ios::end);
	streampos tamanioArchivo=archivo.tellg();
	archivo.seekg(0, ios::beg);
	cantidad=(int)(tamanioArchivo/sizeof(Evento));
	if(cantidad==0) return NULL;
	Evento *eventos=new Evento[cantidad];
	archivo.read((char*)eventos, sizeof(Evento)*cantidad);
	archivo.close();
	return eventos;
}

bool sobreescribirEventos(Evento *eventos, int cantidad){
	ofstream archivo(ARCHIVO_EVENTOS, ios::binary|ios::trunc);
	if(!archivo){
		cout<<"\nError: no se pudo abrir "<<ARCHIVO_EVENTOS;
		return false;
	}
	if(cantidad>0 && eventos!=NULL) archivo.write((char*)eventos, sizeof(Evento)*cantidad);
	archivo.close();
	return true;
}

bool escribirOrganizador(Organizador organizador){
	fs::path ruta(ARCHIVO_ORGANIZADORES);
	if(ruta.has_parent_path()) fs::create_directories(ruta.parent_path());
	ofstream archivo(ARCHIVO_ORGANIZADORES, ios::binary|ios::app);
	if(!archivo){
		cout<<"\nError: no se pudo abrir "<<ARCHIVO_ORGANIZADORES;
		return false;
	}
	archivo.write((char*)&organizador, sizeof(Organizador));
	archivo.close();
	return true;
}

Organizador* leerTodosOrganizadores(int &cantidad){
	cantidad=0;
	ifstream archivo(ARCHIVO_ORGANIZADORES, ios::binary);
	if(!archivo) return NULL;
	archivo.seekg(0, ios::end);
	streampos tamanioArchivo=archivo.tellg();
	archivo.seekg(0, ios::beg);
	cantidad=(int)(tamanioArchivo/sizeof(Organizador));
	if(cantidad==0) return NULL;
	Organizador *organizadores=new Organizador[cantidad];
	archivo.read((char*)organizadores, sizeof(Organizador)*cantidad);
	archivo.close();
	return organizadores;
}

bool sobreescribirOrganizadores(Organizador *organizadores, int cantidad){
	ofstream archivo(ARCHIVO_ORGANIZADORES, ios::binary|ios::trunc);
	if(!archivo){
		cout<<"\nError: no se pudo abrir "<<ARCHIVO_ORGANIZADORES;
		return false;
	}
	if(cantidad>0 && organizadores!=NULL) archivo.write((char*)organizadores, sizeof(Organizador)*cantidad);
	archivo.close();
	return true;
}

int generarID(string nombreArchivo){
	if(nombreArchivo==ARCHIVO_EVENTOS){
		int cantidad=0;
		Evento *eventos=leerTodosEventos(cantidad);
		if(cantidad==0 || eventos==NULL) return 1;
		int ultimoId=eventos[cantidad-1].idEvento;
		delete[] eventos;
		return ultimoId+1;
	}
	else if(nombreArchivo==ARCHIVO_ORGANIZADORES){
		int cantidad=0;
		Organizador *organizadores=leerTodosOrganizadores(cantidad);
		if(cantidad==0 || organizadores==NULL) return 1;
		int ultimoId=organizadores[cantidad-1].idOrganizador;
		delete[] organizadores;
		return ultimoId+1;
	}
	cout<<"\nAdvertencia: generarID no reconoce el archivo "<<nombreArchivo;
	return 1;
}
