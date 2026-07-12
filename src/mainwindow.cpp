/*widgets usados de mainwindow.ui: btnRegistrarEvento, btnEditarEvento, btnInscripciones,
btnReportes, btnAcercaDe, btnSalir.
*/

#include "../include/mainwindow.h"
#include "ui_mainwindow.h"
#include "../include/registrareventowindow.h"
#include "../include/editareventowindow.h"
#include "../include/registrarparticipantewindow.h"
#include "../include/registrarorganizadorwindow.h"
#include "../include/inscripcionwindow.h"
#include "../include/reporteswindow.h"
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

void MainWindow::on_btnRegistrarParticipante_clicked(){
	RegistrarParticipanteWindow ventana(this);
	ventana.exec();
}

void MainWindow::on_btnRegistrarOrganizador_clicked(){
	RegistrarOrganizadorWindow ventana(this);
	ventana.exec();
}

void MainWindow::on_btnInscripciones_clicked(){
	InscripcionWindow *ventana=new InscripcionWindow();
	ventana->setAttribute(Qt::WA_DeleteOnClose);
	ventana->show();
}

void MainWindow::on_btnReportes_clicked(){
	ReportesWindow *ventana=new ReportesWindow();
	ventana->setAttribute(Qt::WA_DeleteOnClose);
	ventana->show();
}

void MainWindow::on_btnAcercaDe_clicked(){
	AcercaDeWindow ventana(this);
	ventana.exec();
}

void MainWindow::on_btnSalir_clicked(){
	close();
}
