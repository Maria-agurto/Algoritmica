/*widgets usados de reportes.ui (version real de Adrian): lblTitulo, btnReporteGeneral,
btnEventoMayorAsistencia, btnEventosDisponibles, btnEventosLlenos, btnPorcentajeOcupacion,
cmbCriterioOrden, cmbOrdenDireccion, btnOrdenar, cmbCriterioFiltro, txtValorFiltro,
btnFiltrar, btnLimpiarFiltro, tablaEventos, lblResultadoTexto, txtResultado,
statusBarReportes.

CORRECCION (integracion): reportes.h/.cpp ya fueron actualizados por Adrian para que
ordenarEventos(int, bool) y filtrarEventos(int, string) reciban los valores como
parametro en vez de pedirlos por consola (cin). Este controlador ahora SI toma los
valores de cmbCriterioOrden/cmbOrdenDireccion y cmbCriterioFiltro/txtValorFiltro y se
los pasa directamente a las funciones.*/

#include "../include/reporteswindow.h"
#include "ui_reportes.h"
#include "../include/reportes.h"
#include "../include/archivos.h"
#include "../include/evento.h"
#include <QTableWidgetItem>
#include <sstream>
#include <iostream>
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

void ReportesWindow::cargarTablaEventos(){
	ui->tablaEventos->setRowCount(0);

	int cantidad=0;
	Evento *eventos=leerTodosEventos(cantidad);
	if(eventos==NULL || cantidad==0) return;

	for(int i=0;i<cantidad;i++){
		int fila=ui->tablaEventos->rowCount();
		ui->tablaEventos->insertRow(fila);

		double ocupacion=0.0;
		if(eventos[i].capacidad>0){
			ocupacion=(double)eventos[i].inscritos/eventos[i].capacidad*100.0;
		}

		ui->tablaEventos->setItem(fila, 0, new QTableWidgetItem(QString::number(eventos[i].idEvento)));
		ui->tablaEventos->setItem(fila, 1, new QTableWidgetItem(QString(eventos[i].titulo)));
		ui->tablaEventos->setItem(fila, 2, new QTableWidgetItem(QString(eventos[i].categoria)));
		ui->tablaEventos->setItem(fila, 3, new QTableWidgetItem(QString(eventos[i].fecha)));
		ui->tablaEventos->setItem(fila, 4, new QTableWidgetItem(QString::number(eventos[i].capacidad)));
		ui->tablaEventos->setItem(fila, 5, new QTableWidgetItem(QString::number(eventos[i].inscritos)));
		ui->tablaEventos->setItem(fila, 6, new QTableWidgetItem(QString(eventos[i].estado)));
		ui->tablaEventos->setItem(fila, 7, new QTableWidgetItem(QString::number(ocupacion,'f',2)+"%"));
	}

	delete[] eventos;
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
	// CORRECCION: se toman el criterio y la direccion
	// directamente de los combos, ya no hace falta avisar
	// que la funcion pide datos por consola.
	int criterio=ui->cmbCriterioOrden->currentIndex()+1; // 1=Titulo 2=Fecha 3=Capacidad 4=Inscritos
	bool ascendente=(ui->cmbOrdenDireccion->currentIndex()==0); // 0=Ascendente 1=Descendente

	ejecutarYMostrar([criterio, ascendente](){ ordenarEventos(criterio, ascendente); });
	cargarTablaEventos();
	ui->statusBarReportes->showMessage("Eventos ordenados correctamente.", 3000);
}

void ReportesWindow::on_btnFiltrar_clicked(){
	// CORRECCION: se toman el criterio y el valor
	// directamente del combo y del QLineEdit.
	int criterio=ui->cmbCriterioFiltro->currentIndex()+1; // 1=Categoria 2=Estado 3=Fecha
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
