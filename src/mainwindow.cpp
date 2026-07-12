/*widgets usados de mainwindow.ui: btnRegistrarEvento, btnEditarEvento, btnInscripciones,
btnReportes, btnAcercaDe, btnSalir.
Este es el modulo de Integracion (Diego): abre las ventanas de cada modulo, pero NO
modifica la logica que ya desarrollaron Maria, Darren y Adrian.*/

#include "../include/mainwindow.h"
#include "ui_mainwindow.h"
#include "../include/registrareventowindow.h"
#include "../include/editareventowindow.h"
#include "../include/inscripcionwindow.h"
#include "../include/reportewindow.h"
#include "../include/acercadewindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow){
	ui->setupUi(this);
	setWindowTitle("Sistema de Gestion de Eventos Academicos");
}

MainWindow::~MainWindow(){
	delete ui;
}

void MainWindow::on_btnRegistrarEvento_clicked(){
	RegistrarEventoWindow ventana(this);
	ventana.exec();
}

void MainWindow::on_btnEditarEvento_clicked(){
	EditarEventoWindow ventana(this);
	ventana.exec();
}

void MainWindow::on_btnInscripciones_clicked(){
	//InscripcionWindow (creado en Integracion, ver inscripcionwindow.h/.cpp)
	//conecta Inscripcion.ui con las funciones de inscripciones.cpp
	//(inscribirParticipante, anularInscripcion, actualizarEstado) sin
	//modificar esa logica.
	InscripcionWindow *ventana=new InscripcionWindow();
	ventana->setAttribute(Qt::WA_DeleteOnClose);
	ventana->show();
}

void MainWindow::on_btnReportes_clicked(){
	//ReporteWindow (creado en Integracion, ver reportewindow.h/.cpp) conecta
	//reporte.ui con las funciones de reportes.cpp, capturando su salida por
	//consola (cout) y mostrandola en un QTextEdit, sin modificar esa logica.
	//Ordenar/Filtrar siguen pidiendo datos por consola (cin); ver el aviso
	//que se muestra en esos dos botones dentro de reportewindow.cpp.
	ReporteWindow ventana(this);
	ventana.exec();
}

void MainWindow::on_btnAcercaDe_clicked(){
	AcercaDeWindow ventana(this);
	ventana.exec();
}

void MainWindow::on_btnSalir_clicked(){
	close();
}
