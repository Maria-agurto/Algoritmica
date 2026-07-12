/*widgets usados de Inscripcion.ui: lineEditCodigoAlumno, spinBoxIdEvento, botonInscribir,
botonAnular, botonActualizarAsistencia, tablaInscripciones, labelMensaje.

CORRECCION DE INTEGRACION: inscripciones.h ahora expone listarInscripciones()
(ver inscripciones.h/.cpp), asi que este controlador ya NO reimplementa su propia
copia de la lectura binaria de data/inscripciones.dat. Antes, si el formato de
guardado cambiaba en inscripciones.cpp, esta ventana podia romperse en silencio
porque nadie actualizaba su copia duplicada del parser.*/

#include "../include/inscripcionwindow.h"
#include "ui_Inscripcion.h"
#include "../include/inscripciones.h"
#include "../include/inscripcion.h"
#include <QTableWidgetItem>
#include <QMessageBox>
#include <string>
using namespace std;

InscripcionWindow::InscripcionWindow(QWidget *parent):QWidget(parent), ui(new Ui::Inscripcion){
	ui->setupUi(this);
	ui->tablaInscripciones->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tablaInscripciones->setEditTriggers(QAbstractItemView::NoEditTriggers);
	cargarTablaInscripciones();
}

InscripcionWindow::~InscripcionWindow(){
	delete ui;
}

void InscripcionWindow::cargarTablaInscripciones(){
	ui->tablaInscripciones->setRowCount(0);

	vector<Inscripcion> lista=listarInscripciones();
	int fila=0;
	for(const Inscripcion &actual : lista){
		ui->tablaInscripciones->insertRow(fila);
		ui->tablaInscripciones->setItem(fila, 0, new QTableWidgetItem(QString::number(actual.idInscripcion)));
		ui->tablaInscripciones->setItem(fila, 1, new QTableWidgetItem(QString::number(actual.idEvento)));
		ui->tablaInscripciones->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(actual.codigoAlumno)));
		ui->tablaInscripciones->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(actual.fechaRegistro)));
		ui->tablaInscripciones->setItem(fila, 4, new QTableWidgetItem(actual.asistencia ? "Si" : "No"));
		fila++;
	}
}

int InscripcionWindow::idInscripcionSeleccionado(){
	QList<QTableWidgetItem*> seleccion=ui->tablaInscripciones->selectedItems();
	if(seleccion.isEmpty()) return -1;
	int fila=seleccion.first()->row();
	return ui->tablaInscripciones->item(fila,0)->text().toInt();
}

void InscripcionWindow::on_botonInscribir_clicked(){
	string codigoAlumno=ui->lineEditCodigoAlumno->text().toStdString();
	int idEvento=ui->spinBoxIdEvento->value();

	bool ok=inscribirParticipante(codigoAlumno, idEvento);
	if(!ok){
		ui->labelMensaje->setText("No se pudo inscribir: verifique que el codigo y el ID de evento existan, que haya cupo y que no este ya inscrito.");
		return;
	}

	ui->labelMensaje->setText("Inscripcion registrada correctamente.");
	ui->lineEditCodigoAlumno->clear();
	cargarTablaInscripciones();
}

void InscripcionWindow::on_botonAnular_clicked(){
	int id=idInscripcionSeleccionado();
	if(id==-1){
		ui->labelMensaje->setText("Seleccione una fila de la tabla para anular.");
		return;
	}

	bool ok=anularInscripcion(id);
	ui->labelMensaje->setText(ok ? "Inscripcion anulada correctamente." : "No se encontro la inscripcion seleccionada.");
	if(ok) cargarTablaInscripciones();
}

void InscripcionWindow::on_botonActualizarAsistencia_clicked(){
	int id=idInscripcionSeleccionado();
	if(id==-1){
		ui->labelMensaje->setText("Seleccione una fila de la tabla para marcar asistencia.");
		return;
	}

	bool ok=actualizarEstado(id, true);
	ui->labelMensaje->setText(ok ? "Asistencia actualizada correctamente." : "No se encontro la inscripcion seleccionada.");
	if(ok) cargarTablaInscripciones();
}
