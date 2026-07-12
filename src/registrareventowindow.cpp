/*widgets usados de registrarEvento.ui: txtTitulo, txtCategoria, dateFecha, timeHoraInicio,
timeHoraFin, spinCapacidad, btnGuardar, btnCancelar.*/

#include "../include/registrareventowindow.h"
#include "ui_registrarEvento.h"
#include "../include/evento.h"
#include "../include/eventos.h"
#include "../include/validaciones.h"
#include <QMessageBox>
#include <cstring>
using namespace std;

RegistrarEventoWindow::RegistrarEventoWindow(QWidget *parent):QDialog(parent), ui(new Ui::RegistrarEventoWindow), idOrganizadorAsignado(1){
	ui->setupUi(this);
}

RegistrarEventoWindow::~RegistrarEventoWindow(){
	delete ui;
}

void RegistrarEventoWindow::setIdOrganizador(int idOrganizador){
	idOrganizadorAsignado=idOrganizador;
}

bool RegistrarEventoWindow::validarCampos(){
	string titulo=ui->txtTitulo->text().toStdString();
	string categoria=ui->txtCategoria->text().toStdString();
	string fecha=ui->dateFecha->date().toString("dd/MM/yyyy").toStdString();
	string horaInicio=ui->timeHoraInicio->time().toString("HH:mm").toStdString();
	string horaFin=ui->timeHoraFin->time().toString("HH:mm").toStdString();
	int capacidad=ui->spinCapacidad->value();

	if(!validarCampoVacio(titulo)){
		QMessageBox::warning(this,"Campo vacio","El titulo del evento no puede estar vacio.");
		return false;
	}
	if(!validarTexto(titulo, TITULO_MAX-1)){
		QMessageBox::warning(this,"Titulo invalido","El titulo excede el limite de 100 caracteres.");
		return false;
	}
	if(!validarCampoVacio(categoria)){
		QMessageBox::warning(this,"Campo vacio","La categoria no puede estar vacia.");
		return false;
	}
	if(!validarTexto(categoria, CATEGORIA_MAX-1)){
		QMessageBox::warning(this,"Categoria invalida","La categoria excede el limite de caracteres permitido.");
		return false;
	}
	if(!validarFecha(fecha)){
		QMessageBox::warning(this,"Fecha invalida","La fecha ingresada no es valida.");
		return false;
	}
	if(!validarHora(horaInicio, horaFin)){
		QMessageBox::warning(this,"Hora invalida","La hora fin debe ser mayor que la hora inicio.");
		return false;
	}
	if(!validarNumero(capacidad)){
		QMessageBox::warning(this,"Capacidad invalida","La capacidad debe ser mayor que 0.");
		return false;
	}
	return true;
}

void RegistrarEventoWindow::on_btnGuardar_clicked(){
	if(!validarCampos()) return;

	string titulo=ui->txtTitulo->text().toStdString();
	string categoria=ui->txtCategoria->text().toStdString();
	string fecha=ui->dateFecha->date().toString("dd/MM/yyyy").toStdString();
	string horaInicio=ui->timeHoraInicio->time().toString("HH:mm").toStdString();
	string horaFin=ui->timeHoraFin->time().toString("HH:mm").toStdString();

	Evento nuevoEvento={};
	strncpy(nuevoEvento.titulo, titulo.c_str(), TITULO_MAX-1);
	nuevoEvento.titulo[TITULO_MAX-1]='\0';
	strncpy(nuevoEvento.categoria, categoria.c_str(), CATEGORIA_MAX-1);
	nuevoEvento.categoria[CATEGORIA_MAX-1]='\0';
	strncpy(nuevoEvento.fecha, fecha.c_str(), FECHA_MAX-1);
	nuevoEvento.fecha[FECHA_MAX-1]='\0';
	strncpy(nuevoEvento.horaInicio, horaInicio.c_str(), HORA_MAX-1);
	nuevoEvento.horaInicio[HORA_MAX-1]='\0';
	strncpy(nuevoEvento.horaFin, horaFin.c_str(), HORA_MAX-1);
	nuevoEvento.horaFin[HORA_MAX-1]='\0';
	nuevoEvento.capacidad=ui->spinCapacidad->value();
	nuevoEvento.inscritos=0;
	strncpy(nuevoEvento.estado, "Disponible", ESTADO_MAX-1);
	nuevoEvento.estado[ESTADO_MAX-1]='\0';
	nuevoEvento.idOrganizador=idOrganizadorAsignado;

	int idGenerado=registrarEvento(nuevoEvento);

	if(idGenerado==-1){
		QMessageBox::critical(this,"Error","No se pudo guardar el evento. Verifique que la carpeta 'data' sea accesible desde donde corre la aplicacion (revise la consola para mas detalle).");
		return;
	}

	QMessageBox::information(this,"Exito","Evento registrado correctamente con ID: "+QString::number(idGenerado));
	accept();
}

void RegistrarEventoWindow::on_btnCancelar_clicked(){
	reject();
}
