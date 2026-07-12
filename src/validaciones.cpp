#include "../include/validaciones.h"
#include <cctype>
using namespace std;

bool validarTexto(string texto, int maxLen){
	if(texto.empty()) return false;
	if((int)texto.length()>maxLen) return false;
	return true;
}

bool validarNumero(int numero){
	return numero>0;
}

bool validarFecha(string fecha){
	if(fecha.length()!=10) return false;
	if(fecha[2]!='/' || fecha[5]!='/') return false;
	for(int i=0;i<10;i++){
		if(i==2 || i==5) continue;
		if(!isdigit(fecha[i])) return false;
	}
	int dia=stoi(fecha.substr(0,2));
	int mes=stoi(fecha.substr(3,2));
	int anio=stoi(fecha.substr(6,4));
	if(mes<1 || mes>12) return false;
	if(anio<1900 || anio>2100) return false;
	int diasPorMes[]={31,28,31,30,31,30,31,31,30,31,30,31};
	bool esBisiesto=(anio%4==0 && anio%100!=0) || (anio%400==0);
	if(mes==2 && esBisiesto) diasPorMes[1]=29;
	if(dia<1 || dia>diasPorMes[mes-1]) return false;
	return true;
}

bool parsearHora(string hora, int &minutosTotales){
	if(hora.length()!=5) return false;
	if(hora[2]!=':') return false;
	for(int i=0;i<5;i++){
		if(i==2) continue;
		if(!isdigit(hora[i])) return false;
	}
	int horas=stoi(hora.substr(0,2));
	int minutos=stoi(hora.substr(3,2));
	if(horas<0 || horas>23) return false;
	if(minutos<0 || minutos>59) return false;
	minutosTotales=horas*60+minutos;
	return true;
}

bool validarHora(string horaInicio, string horaFin){
	int minutosInicio=0, minutosFin=0;
	if(!parsearHora(horaInicio,minutosInicio)) return false;
	if(!parsearHora(horaFin,minutosFin)) return false;
	return minutosFin>minutosInicio;
}

bool validarCampoVacio(string texto){
	if(texto.empty()) return false;
	for(char c:texto){
		if(!isspace((unsigned char)c)) return true;
	}
	return false;
}
