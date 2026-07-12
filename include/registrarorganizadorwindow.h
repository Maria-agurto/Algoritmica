#ifndef REGISTRARORGANIZADORWINDOW_H
#define REGISTRARORGANIZADORWINDOW_H

#include <QDialog>

namespace Ui{
class RegistrarOrganizadorWindow;
}

//====================================================
// Ventana para registrar un nuevo Organizador.
// Conecta ui/registrarOrganizador.ui con
// escribirOrganizador() y generarID() de archivos.h
// (Maria), sin modificar esa logica.
//
// Antes de esto, RegistrarEventoWindow siempre usaba
// idOrganizadorAsignado=1 a falta de organizadores
// reales registrados en organizadores.dat.
//====================================================
class RegistrarOrganizadorWindow : public QDialog{
	Q_OBJECT

public:
	explicit RegistrarOrganizadorWindow(QWidget *parent=nullptr);
	~RegistrarOrganizadorWindow();

	//ID asignado al ultimo organizador guardado con exito,
	//por si quien abre esta ventana (ej. RegistrarEventoWindow)
	//quiere usarlo de inmediato.
	int idOrganizadorRegistrado() const;

private slots:
	void on_btnGuardar_clicked();
	void on_btnCancelar_clicked();

private:
	Ui::RegistrarOrganizadorWindow *ui;
	int idGuardado;
	bool validarCampos();
};

#endif
