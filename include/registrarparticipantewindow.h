#ifndef REGISTRARPARTICIPANTEWINDOW_H
#define REGISTRARPARTICIPANTEWINDOW_H

#include <QDialog>

namespace Ui{
class RegistrarParticipanteWindow;
}

class RegistrarParticipanteWindow : public QDialog{
	Q_OBJECT

public:
	explicit RegistrarParticipanteWindow(QWidget *parent=nullptr);
	~RegistrarParticipanteWindow();

private slots:
	void on_btnGuardar_clicked();
	void on_btnCancelar_clicked();
	void on_cmbCarrera_currentIndexChanged(int index);
	void on_cmbFacultad_currentIndexChanged(int index);

private:
	Ui::RegistrarParticipanteWindow *ui;
	bool validarCampos();
};

#endif
