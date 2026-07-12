/*widgets usados de acercaDe.ui: lblNombreProyecto, lblDescripcion, lblIntegrantes,
lblVersion, btnCerrar.*/

#include "../include/acercadewindow.h"
#include "ui_acercaDe.h"

AcercaDeWindow::AcercaDeWindow(QWidget *parent):QDialog(parent), ui(new Ui::AcercaDeWindow){
	ui->setupUi(this);
}

AcercaDeWindow::~AcercaDeWindow(){
	delete ui;
}

void AcercaDeWindow::on_btnCerrar_clicked(){
	accept();
}
