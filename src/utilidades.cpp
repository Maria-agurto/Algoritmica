#include "../include/utilidades.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cstdlib>
using namespace std;

string obtenerFechaActual(){
	time_t ahora=time(0);
	tm *fechaLocal=localtime(&ahora);
	return formatearFecha(fechaLocal->tm_mday, fechaLocal->tm_mon+1, fechaLocal->tm_year+1900);
}

string formatearFecha(int dia, int mes, int anio){
	ostringstream salida;
	salida<<setw(2)<<setfill('0')<<dia<<"/"
		<<setw(2)<<setfill('0')<<mes<<"/"
		<<anio;
	return salida.str();
}

string convertirMayusculas(string texto){
	for(size_t i=0;i<texto.length();i++){
		texto[i]=toupper((unsigned char)texto[i]);
	}
	return texto;
}

string trim(string texto){
	size_t inicio=texto.find_first_not_of(" \t\n\r");
	if(inicio==string::npos) return "";
	size_t fin=texto.find_last_not_of(" \t\n\r");
	return texto.substr(inicio, fin-inicio+1);
}

string generarCodigo(int longitud){
	static const string caracteres="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	static bool semillaInicializada=false;
	if(!semillaInicializada){
		srand((unsigned int)time(0));
		semillaInicializada=true;
	}
	string codigo="";
	for(int i=0;i<longitud;i++){
		codigo+=caracteres[rand()%caracteres.length()];
	}
	return codigo;
}
