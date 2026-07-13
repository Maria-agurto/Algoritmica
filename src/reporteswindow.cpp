#include "../include/reporteswindow.h"
#include "ui_reportes.h"
#include "../include/reportes.h"
#include "../include/archivos.h"
#include "../include/evento.h"
#include <QTableWidgetItem>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

ReportesWindow::ReportesWindow(QWidget *parent):QWidget(parent), ui(new Ui::ReportesWindow){
	ui->setupUi(this);
	cargarTablaEventos();
}

ReportesWindow::~ReportesWindow(){
	delete ui;
}

void ReportesWindow::ejecutarYMostrar(const std::function<void()> &funcionReporte){
	ostringstream bufferSalida;
	streambuf *coutOriginal=cout.rdbuf(bufferSalida.rdbuf());

	funcionReporte();

	cout.rdbuf(coutOriginal);
	ui->txtResultado->setPlainText(QString::fromStdString(bufferSalida.str()));
}

void ReportesWindow::cargarTablaEventos(const std::vector<Evento> &eventosOrdenados){
	ui->tablaEventos->setRowCount(0);

	vector<Evento> lista;
	if(!eventosOrdenados.empty()){
		lista=eventosOrdenados;
	}else{
		int cantidad=0;
		Evento *eventos=leerTodosEventos(cantidad);
		if(eventos==NULL || cantidad==0) return;
		for(int i=0;i<cantidad;i++) lista.push_back(eventos[i]);
		delete[] eventos;
	}

	for(size_t i=0;i<lista.size();i++){
		int fila=ui->tablaEventos->rowCount();
		ui->tablaEventos->insertRow(fila);

		double ocupacion=0.0;
		if(lista[i].capacidad>0){
			ocupacion=(double)lista[i].inscritos/lista[i].capacidad*100.0;
		}

		ui->tablaEventos->setItem(fila, 0, new QTableWidgetItem(QString::number(lista[i].idEvento)));
		ui->tablaEventos->setItem(fila, 1, new QTableWidgetItem(QString(lista[i].titulo)));
		ui->tablaEventos->setItem(fila, 2, new QTableWidgetItem(QString(lista[i].categoria)));
		ui->tablaEventos->setItem(fila, 3, new QTableWidgetItem(QString(lista[i].fecha)));
		ui->tablaEventos->setItem(fila, 4, new QTableWidgetItem(QString::number(lista[i].capacidad)));
		ui->tablaEventos->setItem(fila, 5, new QTableWidgetItem(QString::number(lista[i].inscritos)));
		ui->tablaEventos->setItem(fila, 6, new QTableWidgetItem(QString(lista[i].estado)));
		ui->tablaEventos->setItem(fila, 7, new QTableWidgetItem(QString::number(ocupacion,'f',2)+"%"));
	}
}

void ReportesWindow::on_btnReporteGeneral_clicked(){
	ejecutarYMostrar(generarReporteGeneral);
}

void ReportesWindow::on_btnEventoMayorAsistencia_clicked(){
	ejecutarYMostrar(eventoMayorAsistencia);
}

void ReportesWindow::on_btnEventosDisponibles_clicked(){
	ejecutarYMostrar(eventosDisponibles);
	cargarTablaEventos();
}

void ReportesWindow::on_btnEventosLlenos_clicked(){
	ejecutarYMostrar(eventosLlenos);
	cargarTablaEventos();
}

void ReportesWindow::on_btnPorcentajeOcupacion_clicked(){
	ejecutarYMostrar(porcentajeOcupacion);
}

void ReportesWindow::on_btnOrdenar_clicked(){
	int criterio=ui->cmbCriterioOrden->currentIndex()+1; //1Titulo 2Fecha 3Capacidad 4Inscritos
	bool ascendente=(ui->cmbOrdenDireccion->currentIndex()==0); //0ascendente 1descendente

	ejecutarYMostrar([criterio, ascendente](){ ordenarEventos(criterio, ascendente); });
	int cantidad=0;
	Evento *eventosCrudos=leerTodosEventos(cantidad);
	vector<Evento> eventosOrdenados;
	if(eventosCrudos!=NULL){
		for(int i=0;i<cantidad;i++) eventosOrdenados.push_back(eventosCrudos[i]);
		delete[] eventosCrudos;
	}
	sort(eventosOrdenados.begin(), eventosOrdenados.end(), [criterio, ascendente](const Evento &a, const Evento &b){
		bool resultado;
		switch(criterio){
			case 1: resultado=strcmp(a.titulo, b.titulo)<0; break;
			case 2: resultado=strcmp(a.fecha, b.fecha)<0; break;
			case 3: resultado=a.capacidad<b.capacidad; break;
			case 4: resultado=a.inscritos<b.inscritos; break;
			default: resultado=a.idEvento<b.idEvento; break;
		}
		return ascendente ? resultado : !resultado;
	});
	cargarTablaEventos(eventosOrdenados);

	ui->statusBarReportes->showMessage("Eventos ordenados correctamente.", 3000);
}

void ReportesWindow::on_btnFiltrar_clicked(){
	int criterio=ui->cmbCriterioFiltro->currentIndex()+1; //1Categoria 2Estado 3Fecha
	string valor=ui->txtValorFiltro->text().toStdString();

	ejecutarYMostrar([criterio, valor](){ filtrarEventos(criterio, valor); });
	ui->statusBarReportes->showMessage("Filtro aplicado.", 3000);
}

void ReportesWindow::on_btnLimpiarFiltro_clicked(){
	ui->cmbCriterioFiltro->setCurrentIndex(0);
	ui->txtValorFiltro->clear();
	ui->txtResultado->clear();
	cargarTablaEventos();
	ui->statusBarReportes->showMessage("Filtro limpiado.", 3000);
}
