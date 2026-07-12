#include "../include/registrarorganizadorwindow.h"
#include "ui_registrarOrganizador.h"
#include "../include/organizador.h"
#include "../include/archivos.h"
#include "../include/validaciones.h"
#include <cstring>
using namespace std;

RegistrarOrganizadorWindow::RegistrarOrganizadorWindow(QWidget *parent):QDialog(parent), ui(new Ui::RegistrarOrganizadorWindow), idGuardado(-1){
	ui->setupUi(this);
}

RegistrarOrganizadorWindow::~RegistrarOrganizadorWindow(){
	delete ui;
}

int RegistrarOrganizadorWindow::idOrganizadorRegistrado() const{
	return idGuardado;
}

bool RegistrarOrganizadorWindow::validarCampos(){
	string nombre=ui->txtNombre->text().toStdString();
	string departamento=ui->txtDepartamento->text().toStdString();

	if(!validarCampoVacio(nombre) || !validarTexto(nombre, NOMBRE_MAX-1)){
		ui->lblMensaje->setText("El nombre no puede estar vacio y debe tener menos de 80 caracteres.");
		return false;
	}
	if(!validarCampoVacio(departamento) || !validarTexto(departamento, DEPARTAMENTO_MAX-1)){
		ui->lblMensaje->setText("El departamento no puede estar vacio y debe tener menos de 60 caracteres.");
		return false;
	}
	return true;
}

void RegistrarOrganizadorWindow::on_btnGuardar_clicked(){
	if(!validarCampos()) return;

	string nombre=ui->txtNombre->text().toStdString();
	string departamento=ui->txtDepartamento->text().toStdString();

	Organizador nuevo={};
	nuevo.idOrganizador=generarID(ARCHIVO_ORGANIZADORES);
	strncpy(nuevo.nombre, nombre.c_str(), NOMBRE_MAX-1);
	nuevo.nombre[NOMBRE_MAX-1]='\0';
	strncpy(nuevo.departamento, departamento.c_str(), DEPARTAMENTO_MAX-1);
	nuevo.departamento[DEPARTAMENTO_MAX-1]='\0';

	bool ok=escribirOrganizador(nuevo);
	if(!ok){
		ui->lblMensaje->setText("No se pudo guardar el organizador. Revise la consola.");
		return;
	}

	idGuardado=nuevo.idOrganizador;
	accept();
}

void RegistrarOrganizadorWindow::on_btnCancelar_clicked(){
	reject();
}
