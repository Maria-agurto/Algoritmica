/*widgets usados de reporte.ui: btnReporteGeneral, btnMayorAsistencia, btnDisponibles,
btnLlenos, btnPorcentajeOcupacion, btnOrdenar, btnFiltrar, txtResultado, btnCerrar.

NOTA DE INTEGRACION IMPORTANTE:
Las funciones de reportes.cpp (Adrian) imprimen directamente con cout, en vez de
retornar un string. Para no modificar esa logica, este controlador redirige
temporalmente std::cout hacia un buffer en memoria mientras se ejecuta la funcion,
y despues muestra ese buffer en txtResultado.

Ademas, ordenarEventos() y filtrarEventos() piden datos con getline(cin,...), es
decir, siguen esperando entrada por CONSOLA (terminal), no por la ventana Qt. Como
integrador no puedo inventar esos criterios de entrada sin tocar reportes.cpp, asi
que se avisa al usuario que revise la consola donde corre la aplicacion para escribir
la opcion pedida. Lo ideal (a futuro, y lo decide Adrian) es que reportes.cpp reciba
esos criterios como parametros de funcion en vez de leerlos con cin.*/

#include "../include/reportewindow.h"
#include "ui_reporte.h"
#include "../include/reportes.h"
#include <QMessageBox>
#include <sstream>
#include <iostream>

ReporteWindow::ReporteWindow(QWidget *parent):QDialog(parent), ui(new Ui::ReporteWindow){
	ui->setupUi(this);
}

ReporteWindow::~ReporteWindow(){
	delete ui;
}

void ReporteWindow::ejecutarYMostrar(const std::function<void()> &funcionReporte){
	std::ostringstream bufferSalida;
	std::streambuf *coutOriginal=std::cout.rdbuf(bufferSalida.rdbuf());

	funcionReporte();

	std::cout.rdbuf(coutOriginal);
	ui->txtResultado->setPlainText(QString::fromStdString(bufferSalida.str()));
}

void ReporteWindow::on_btnReporteGeneral_clicked(){
	ejecutarYMostrar(generarReporteGeneral);
}

void ReporteWindow::on_btnMayorAsistencia_clicked(){
	ejecutarYMostrar(eventoMayorAsistencia);
}

void ReporteWindow::on_btnDisponibles_clicked(){
	ejecutarYMostrar(eventosDisponibles);
}

void ReporteWindow::on_btnLlenos_clicked(){
	ejecutarYMostrar(eventosLlenos);
}

void ReporteWindow::on_btnPorcentajeOcupacion_clicked(){
	ejecutarYMostrar(porcentajeOcupacion);
}

void ReporteWindow::on_btnOrdenar_clicked(){
	QMessageBox::information(this,"Entrada por consola",
		"Esta funcion pide el criterio y el orden escribiendo en la consola (terminal) donde se ejecuto la aplicacion. Revise esa ventana para continuar.");
	ejecutarYMostrar(ordenarEventos);
}

void ReporteWindow::on_btnFiltrar_clicked(){
	QMessageBox::information(this,"Entrada por consola",
		"Esta funcion pide el criterio y el valor a buscar escribiendo en la consola (terminal) donde se ejecuto la aplicacion. Revise esa ventana para continuar.");
	ejecutarYMostrar(filtrarEventos);
}

void ReporteWindow::on_btnCerrar_clicked(){
	accept();
}
