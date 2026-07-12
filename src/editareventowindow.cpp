#include "../include/editareventowindow.h"
#include "ui_editarEvento.h"
#include "../include/eventos.h"
#include "../include/validaciones.h"
#include <QMessageBox>
#include <QDate>
#include <QTime>
#include <cstring>
using namespace std;

EditarEventoWindow::EditarEventoWindow(QWidget *parent):QDialog(parent), ui(new Ui::EditarEventoWindow), idEventoActual(-1){
	ui->setupUi(this);
}

EditarEventoWindow::~EditarEventoWindow(){
	delete ui;
}

void EditarEventoWindow::cargarDatosEnFormulario(const Evento &evento){
	ui->txtNuevoTitulo->setText(QString(evento.titulo));
	ui->txtNuevaCategoria->setText(QString(evento.categoria));
	ui->dateNuevaFecha->setDate(QDate::fromString(QString(evento.fecha), "dd/MM/yyyy"));
	ui->timeNuevaHoraInicio->setTime(QTime::fromString(QString(evento.horaInicio), "HH:mm"));
	ui->timeNuevaHoraFin->setTime(QTime::fromString(QString(evento.horaFin), "HH:mm"));
	ui->spinNuevaCapacidad->setValue(evento.capacidad);
}

void EditarEventoWindow::on_btnBuscar_clicked(){
	bool esNumero=false;
	int id=ui->txtBuscarID->text().toInt(&esNumero);

	if(!esNumero || !validarNumero(id)){
		QMessageBox::warning(this,"ID invalido","Ingrese un ID de evento numerico valido.");
		return;
	}

	Evento evento=buscarEvento(id);
	if(evento.idEvento==-1){
		QMessageBox::warning(this,"No encontrado","No existe un evento con el ID "+QString::number(id));
		idEventoActual=-1;
		return;
	}

	idEventoActual=id;
	cargarDatosEnFormulario(evento);
}

bool EditarEventoWindow::validarCampos(){
	string titulo=ui->txtNuevoTitulo->text().toStdString();
	string categoria=ui->txtNuevaCategoria->text().toStdString();
	string fecha=ui->dateNuevaFecha->date().toString("dd/MM/yyyy").toStdString();
	string horaInicio=ui->timeNuevaHoraInicio->time().toString("HH:mm").toStdString();
	string horaFin=ui->timeNuevaHoraFin->time().toString("HH:mm").toStdString();
	int capacidad=ui->spinNuevaCapacidad->value();

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

	Evento original=buscarEvento(idEventoActual);
	if(original.idEvento!=-1 && capacidad<original.inscritos){
		QMessageBox::warning(this,"Capacidad invalida","La capacidad no puede ser menor a los inscritos actuales ("+QString::number(original.inscritos)+").");
		return false;
	}
	return true;
}

void EditarEventoWindow::on_btnGuardarCambios_clicked(){
	if(idEventoActual==-1){
		QMessageBox::warning(this,"Sin evento","Primero busque un evento valido por su ID.");
		return;
	}
	if(!validarCampos()) return;

	Evento original=buscarEvento(idEventoActual);
	if(original.idEvento==-1){
		QMessageBox::critical(this,"Error","El evento ya no existe.");
		idEventoActual=-1;
		return;
	}

	string titulo=ui->txtNuevoTitulo->text().toStdString();
	string categoria=ui->txtNuevaCategoria->text().toStdString();
	string fecha=ui->dateNuevaFecha->date().toString("dd/MM/yyyy").toStdString();
	string horaInicio=ui->timeNuevaHoraInicio->time().toString("HH:mm").toStdString();
	string horaFin=ui->timeNuevaHoraFin->time().toString("HH:mm").toStdString();
	int capacidad=ui->spinNuevaCapacidad->value();

	Evento eventoModificado=original; //conserva idOrganizador, inscritos y estado como base
	strncpy(eventoModificado.titulo, titulo.c_str(), TITULO_MAX-1);
	eventoModificado.titulo[TITULO_MAX-1]='\0';
	strncpy(eventoModificado.categoria, categoria.c_str(), CATEGORIA_MAX-1);
	eventoModificado.categoria[CATEGORIA_MAX-1]='\0';
	strncpy(eventoModificado.fecha, fecha.c_str(), FECHA_MAX-1);
	eventoModificado.fecha[FECHA_MAX-1]='\0';
	strncpy(eventoModificado.horaInicio, horaInicio.c_str(), HORA_MAX-1);
	eventoModificado.horaInicio[HORA_MAX-1]='\0';
	strncpy(eventoModificado.horaFin, horaFin.c_str(), HORA_MAX-1);
	eventoModificado.horaFin[HORA_MAX-1]='\0';
	eventoModificado.capacidad=capacidad;

	//recalcula el estado segun cupos, salvo que ya este Finalizado o Cancelado
	if(strcmp(eventoModificado.estado,"Finalizado")!=0 && strcmp(eventoModificado.estado,"Cancelado")!=0){
		string nuevoEstado=(eventoModificado.inscritos>=eventoModificado.capacidad)?"Lleno":"Disponible";
		strncpy(eventoModificado.estado, nuevoEstado.c_str(), ESTADO_MAX-1);
		eventoModificado.estado[ESTADO_MAX-1]='\0';
	}

	bool ok=editarEvento(idEventoActual, eventoModificado);
	if(!ok){
		QMessageBox::critical(this,"Error","No se pudo actualizar el evento.");
		return;
	}

	QMessageBox::information(this,"Exito","Evento actualizado correctamente.");
	accept();
}

void EditarEventoWindow::on_btnCancelar_clicked(){
	reject();
}

void EditarEventoWindow::on_btnCancelarEvento_clicked(){
	if(idEventoActual==-1){
		QMessageBox::warning(this,"Sin evento","Primero busque un evento valido por su ID.");
		return;
	}

	Evento original=buscarEvento(idEventoActual);
	if(original.idEvento==-1){
		QMessageBox::critical(this,"Error","El evento ya no existe.");
		idEventoActual=-1;
		return;
	}

	QMessageBox::StandardButton respuesta=QMessageBox::question(
		this,
		"Confirmar cancelacion",
		"¿Esta seguro que desea cancelar el evento \""+QString(original.titulo)+"\"?\n\n"
		"El evento sera eliminado permanentemente y ya no se podra editar ni recibir nuevas inscripciones.",
		QMessageBox::Yes|QMessageBox::No,
		QMessageBox::No
	);

	if(respuesta!=QMessageBox::Yes) return;

	bool ok=eliminarEvento(idEventoActual);
	if(!ok){
		QMessageBox::critical(this,"Error","No se pudo cancelar el evento.");
		return;
	}

	QMessageBox::information(this,"Evento cancelado","El evento ha sido cancelado y eliminado correctamente.");
	idEventoActual=-1;
	accept();
}
