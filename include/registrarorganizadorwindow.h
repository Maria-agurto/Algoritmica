#ifndef REGISTRARORGANIZADORWINDOW_H
#define REGISTRARORGANIZADORWINDOW_H

#include <QDialog>

namespace Ui{
class RegistrarOrganizadorWindow;
}

class RegistrarOrganizadorWindow : public QDialog{
	Q_OBJECT

public:
	explicit RegistrarOrganizadorWindow(QWidget *parent=nullptr);
	~RegistrarOrganizadorWindow();

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
