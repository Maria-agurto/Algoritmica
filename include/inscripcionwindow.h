#ifndef INSCRIPCIONWINDOW_H
#define INSCRIPCIONWINDOW_H

#include <QWidget>

namespace Ui{
class Inscripcion;
}

class InscripcionWindow : public QWidget{
	Q_OBJECT

public:
	explicit InscripcionWindow(QWidget *parent=nullptr);
	~InscripcionWindow();

private slots:
	void on_botonInscribir_clicked();
	void on_botonAnular_clicked();
	void on_botonActualizarAsistencia_clicked();

private:
	Ui::Inscripcion *ui;

	void cargarTablaInscripciones();
	int idInscripcionSeleccionado();
};

#endif
