#include "../include/registrarparticipantewindow.h"
#include "ui_registrarParticipante.h"
#include "../include/participante.h"
#include "../include/participantes.h"
#include "../include/validaciones.h"
#include <QInputDialog>
#include <cctype>
using namespace std;

static bool esSoloDigitos(const string &texto){
	if(texto.empty()) return false;
	for(char c : texto){
		if(!isdigit((unsigned char)c)) return false;
	}
	return true;
}

static bool esSoloLetras(const string &texto){
	if(texto.empty()) return false;
	for(char c : texto){
		if(!isalpha((unsigned char)c) && c!=' ') return false;
	}
	return true;
}

RegistrarParticipanteWindow::RegistrarParticipanteWindow(QWidget *parent):QDialog(parent), ui(new Ui::RegistrarParticipanteWindow){
	ui->setupUi(this);
}

RegistrarParticipanteWindow::~RegistrarParticipanteWindow(){
	delete ui;
}

bool RegistrarParticipanteWindow::validarCampos(){
	string codigo=ui->txtCodigo->text().toStdString();
	string nombres=ui->txtNombres->text().toStdString();
	string apellidos=ui->txtApellidos->text().toStdString();

	if(!validarCampoVacio(codigo) || codigo.length()!=8 || !esSoloDigitos(codigo)){
		ui->lblMensaje->setText("El codigo debe tener exactamente 8 digitos.");
		return false;
	}
	if(!validarCampoVacio(nombres) || !esSoloLetras(nombres)){
		ui->lblMensaje->setText("Los nombres solo deben contener letras.");
		return false;
	}
	if(!validarCampoVacio(apellidos) || !esSoloLetras(apellidos)){
		ui->lblMensaje->setText("Los apellidos solo deben contener letras.");
		return false;
	}
	return true;
}

void RegistrarParticipanteWindow::on_btnGuardar_clicked(){
	if(!validarCampos()) return;

	Participante nuevo;
	nuevo.codigo=ui->txtCodigo->text().toStdString();
	nuevo.nombres=ui->txtNombres->text().toStdString();
	nuevo.apellidos=ui->txtApellidos->text().toStdString();
	nuevo.carrera=ui->cmbCarrera->currentText().toStdString();
	nuevo.facultad=ui->cmbFacultad->currentText().toStdString();

	bool ok=registrarParticipante(nuevo);
	if(!ok){
		ui->lblMensaje->setText("No se pudo registrar: el codigo ya existe o esta vacio.");
		return;
	}

	accept();
}

void RegistrarParticipanteWindow::on_btnCancelar_clicked(){
	reject();
}

void RegistrarParticipanteWindow::on_cmbCarrera_currentIndexChanged(int index){
	if(ui->cmbCarrera->itemText(index)!="Otra") return;

	bool ok=false;
	QString texto=QInputDialog::getText(this, "Otra carrera",
		"Escriba el nombre de la carrera:", QLineEdit::Normal, "", &ok).trimmed();

	if(ok && !texto.isEmpty() && texto.length()<=60){
		ui->cmbCarrera->setItemText(index, texto);
	}else{
		ui->cmbCarrera->setCurrentIndex(0);
	}
}

void RegistrarParticipanteWindow::on_cmbFacultad_currentIndexChanged(int index){
	if(ui->cmbFacultad->itemText(index)!="Otra") return;

	bool ok=false;
	QString texto=QInputDialog::getText(this, "Otra facultad",
		"Escriba el nombre de la facultad:", QLineEdit::Normal, "", &ok).trimmed();

	if(ok && !texto.isEmpty() && texto.length()<=60){
		ui->cmbFacultad->setItemText(index, texto);
	}else{
		ui->cmbFacultad->setCurrentIndex(0);
	}
}
