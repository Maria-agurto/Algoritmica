#ifndef EDITAREVENTOWINDOW_H
#define EDITAREVENTOWINDOW_H

#include <QDialog>
#include "evento.h"

namespace Ui{
class EditarEventoWindow;
}

class EditarEventoWindow : public QDialog{
	Q_OBJECT

public:
	explicit EditarEventoWindow(QWidget *parent=nullptr);
	~EditarEventoWindow();

private slots:
	void on_btnBuscar_clicked();
	void on_btnGuardarCambios_clicked();
	void on_btnCancelar_clicked();
	void on_btnCancelarEvento_clicked();

private:
	Ui::EditarEventoWindow *ui;
	int idEventoActual;
	void cargarDatosEnFormulario(const Evento &evento);
	bool validarCampos();
};

#endif
