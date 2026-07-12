#include "../include/participantes.h"
#include <fstream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

static const string ARCHIVO_PARTICIPANTES = "data/participantes.dat";

static void asegurarDirectorioParticipantes(){
	fs::path ruta(ARCHIVO_PARTICIPANTES);
	if(ruta.has_parent_path()) fs::create_directories(ruta.parent_path());
}

static void escribirStringP(ofstream &archivo, const string &texto){
	size_t tamano=texto.size();
	archivo.write(reinterpret_cast<const char*>(&tamano), sizeof(tamano));
	archivo.write(texto.c_str(), static_cast<streamsize>(tamano));
}

static void leerStringP(ifstream &archivo, string &texto){
	size_t tamano=0;
	archivo.read(reinterpret_cast<char*>(&tamano), sizeof(tamano));
	texto.resize(tamano);
	if(tamano>0) archivo.read(&texto[0], static_cast<streamsize>(tamano));
}

static void escribirParticipante(ofstream &archivo, const Participante &p){
	escribirStringP(archivo, p.codigo);
	escribirStringP(archivo, p.nombres);
	escribirStringP(archivo, p.apellidos);
	escribirStringP(archivo, p.carrera);
	escribirStringP(archivo, p.facultad);
}

static bool leerParticipante(ifstream &archivo, Participante &p){
	leerStringP(archivo, p.codigo);
	if(archivo.eof()) return false;
	leerStringP(archivo, p.nombres);
	leerStringP(archivo, p.apellidos);
	leerStringP(archivo, p.carrera);
	leerStringP(archivo, p.facultad);
	return true;
}

vector<Participante> leerTodosParticipantes(){
	vector<Participante> lista;
	ifstream archivo(ARCHIVO_PARTICIPANTES, ios::binary);
	if(!archivo.is_open()) return lista;

	Participante actual;
	while(leerParticipante(archivo, actual)){
		lista.push_back(actual);
	}
	return lista;
}

bool buscarParticipante(const string &codigo, Participante &encontrado){
	vector<Participante> lista=leerTodosParticipantes();
	for(const Participante &p : lista){
		if(p.codigo==codigo){
			encontrado=p;
			return true;
		}
	}
	return false;
}

bool registrarParticipante(const Participante &nuevo){
	if(nuevo.codigo.empty()) return false;

	Participante existente;
	if(buscarParticipante(nuevo.codigo, existente)) return false; //codigo duplicado

	asegurarDirectorioParticipantes();
	ofstream archivo(ARCHIVO_PARTICIPANTES, ios::binary|ios::app);
	if(!archivo.is_open()) return false;
	escribirParticipante(archivo, nuevo);
	archivo.close();
	return true;
}

bool eliminarParticipante(const string &codigo){
	vector<Participante> lista=leerTodosParticipantes();
	vector<Participante> listaActualizada;
	bool encontrado=false;

	for(const Participante &p : lista){
		if(p.codigo==codigo){
			encontrado=true;
			continue;
		}
		listaActualizada.push_back(p);
	}

	if(!encontrado) return false;

	asegurarDirectorioParticipantes();
	ofstream archivo(ARCHIVO_PARTICIPANTES, ios::binary|ios::trunc);
	if(!archivo.is_open()) return false;
	for(const Participante &p : listaActualizada){
		escribirParticipante(archivo, p);
	}
	archivo.close();
	return true;
}
