/*widgets usados de reportes.ui (version real de Adrian): lblTitulo, btnReporteGeneral,
btnEventoMayorAsistencia, btnEventosDisponibles, btnEventosLlenos, btnPorcentajeOcupacion,
cmbCriterioOrden, cmbOrdenDireccion, btnOrdenar, cmbCriterioFiltro, txtValorFiltro,
btnFiltrar, btnLimpiarFiltro, tablaEventos, lblResultadoTexto, txtResultado,
statusBarReportes.

NOTA DE INTEGRACION IMPORTANTE (leer antes de tocar este archivo):
1) Las funciones de reportes.cpp imprimen con cout en vez de retornar datos. Este
   controlador redirige std::cout hacia un buffer en memoria mientras se ejecuta la
   funcion, y muestra ese buffer en txtResultado, sin modificar reportes.cpp/.h.

2) reportes.h todavia declara ordenarEventos() y filtrarEventos() SIN parametros, y
   por dentro siguen usando getline(cin,...) para pedir el criterio/valor. Es decir,
   aunque el .ui ya tiene cmbCriterioOrden/cmbOrdenDireccion y
   cmbCriterioFiltro/txtValorFiltro, ESOS COMBOS TODAVIA NO SE ENVIAN a la funcion,
   porque la funcion no los recibe como parametro; ella sigue leyendo de la consola
   (terminal). Se avisa al usuario por el statusBar. Cuando Adrian actualice
   reportes.h/.cpp para que estas funciones reciban el criterio como parametro (ej.
   ordenarEventos(int criterio, bool ascendente)), aqui solo hay que reemplazar el
   cuerpo de on_btnOrdenar_clicked()/on_btnFiltrar_clicked() por la llamada con esos
   parametros tomados de los combos; no hace falta tocar el resto del archivo.

3) tablaEventos SI se llena correctamente porque se reutiliza leerTodosEventos() de
   archivos.h (Maria), que ya retorna los Evento en memoria; no se duplico logica de
   lectura como tuvo que hacerse para Inscripcion.ui.*/

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
	//Ver NOTA DE INTEGRACION (2) arriba: ordenarEventos() aun no recibe
	//el criterio/direccion elegidos en los combos, sigue pidiendolos
	//por consola.
	ui->statusBarReportes->showMessage(
		"ordenarEventos() aun pide el criterio por consola (terminal); los combos todavia no se envian a la funcion.", 6000);
	ejecutarYMostrar(ordenarEventos);
	cargarTablaEventos();
}

void ReportesWindow::on_btnFiltrar_clicked(){
	//Ver NOTA DE INTEGRACION (2) arriba: filtrarEventos() aun no recibe
	//el criterio/valor elegidos en el combo y el QLineEdit, sigue
	//pidiendolos por consola.
	ui->statusBarReportes->showMessage(
		"filtrarEventos() aun pide el criterio y el valor por consola (terminal); el combo y el campo de texto todavia no se envian a la funcion.", 6000);
	ejecutarYMostrar(filtrarEventos);
}

void ReportesWindow::on_btnLimpiarFiltro_clicked(){
	ui->cmbCriterioFiltro->setCurrentIndex(0);
	ui->txtValorFiltro->clear();
	ui->txtResultado->clear();
	cargarTablaEventos();
	ui->statusBarReportes->showMessage("Filtro limpiado.", 3000);
}
