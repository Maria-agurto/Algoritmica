/*widgets usados de Inscripcion.ui: lineEditCodigoAlumno, spinBoxIdEvento, botonInscribir,
botonAnular, botonActualizarAsistencia, tablaInscripciones, labelMensaje.

NOTA DE INTEGRACION IMPORTANTE:
inscripciones.h (Darren) solo expone inscribirParticipante(), anularInscripcion(),
buscarInscripcion() y actualizarEstado(); no expone una funcion para listar TODAS las
inscripciones (esa lectura completa es una funcion privada dentro de
inscripciones.cpp). Para no modificar ese archivo, este controlador lee
data/inscripciones.dat directamente, replicando el MISMO formato binario que usa
inscripciones.cpp (int idInscripcion, int idEvento, string idEvento con prefijo de
tamano, string fechaRegistro con prefijo de tamano, bool asistencia) unicamente para
poder mostrar la tabla. Si Darren cambia ese formato de guardado, esta lectura debe
actualizarse igual.*/

#include "../include/inscripcionwindow.h"
#include "ui_Inscripcion.h"
#include "../include/inscripciones.h"
#include "../include/inscripcion.h"
#include <QTableWidgetItem>
#include <QMessageBox>
#include <fstream>
#include <string>
using namespace std;

static const string ARCHIVO_INSCRIPCIONES_LECTURA="data/inscripciones.dat";

//----------------------------------------------------
// Lee un string con prefijo de tamano (size_t), igual
// que inscripciones.cpp. Solo para lectura de tabla.
//----------------------------------------------------
static void leerStringLocal(ifstream &archivo, string &texto){
	size_t tamano=0;
	archivo.read(reinterpret_cast<char*>(&tamano), sizeof(tamano));
	texto.resize(tamano);
	if(tamano>0) archivo.read(&texto[0], static_cast<streamsize>(tamano));
}

//----------------------------------------------------
// Lee un registro Inscripcion completo. Retorna false
// al llegar al final del archivo.
//----------------------------------------------------
static bool leerInscripcionLocal(ifstream &archivo, Inscripcion &insc){
	archivo.read(reinterpret_cast<char*>(&insc.idInscripcion), sizeof(insc.idInscripcion));
	if(archivo.eof()) return false;
	archivo.read(reinterpret_cast<char*>(&insc.idEvento), sizeof(insc.idEvento));
	leerStringLocal(archivo, insc.codigoAlumno);
	leerStringLocal(archivo, insc.fechaRegistro);
	archivo.read(reinterpret_cast<char*>(&insc.asistencia), sizeof(insc.asistencia));
	return true;
}

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

	ifstream archivo(ARCHIVO_INSCRIPCIONES_LECTURA, ios::binary);
	if(!archivo.is_open()) return;

	Inscripcion actual;
	int fila=0;
	while(leerInscripcionLocal(archivo, actual)){
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
