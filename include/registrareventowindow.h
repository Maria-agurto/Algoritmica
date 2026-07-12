#ifndef REGISTRAREVENTOWINDOW_H
#define REGISTRAREVENTOWINDOW_H

#include <QDialog>

namespace Ui{
class RegistrarEventoWindow;
}

class RegistrarEventoWindow : public QDialog{
	Q_OBJECT

public:
	explicit RegistrarEventoWindow(QWidget *parent=nullptr);
	~RegistrarEventoWindow();
	void setIdOrganizador(int idOrganizador); 
private slots:
	void on_btnGuardar_clicked();
	void on_btnCancelar_clicked();

private:
	Ui::RegistrarEventoWindow *ui;
	int idOrganizadorAsignado;
	bool validarCampos();
	void cargarOrganizadoresEnCombo(); 
};

#endif
