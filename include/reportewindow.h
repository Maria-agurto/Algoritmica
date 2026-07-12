#ifndef REPORTEWINDOW_H
#define REPORTEWINDOW_H

#include <QDialog>
#include <functional>

namespace Ui{
class ReporteWindow;
}

class ReporteWindow : public QDialog{
	Q_OBJECT

public:
	explicit ReporteWindow(QWidget *parent=nullptr);
	~ReporteWindow();

private slots:
	void on_btnReporteGeneral_clicked();
	void on_btnMayorAsistencia_clicked();
	void on_btnDisponibles_clicked();
	void on_btnLlenos_clicked();
	void on_btnPorcentajeOcupacion_clicked();
	void on_btnOrdenar_clicked();
	void on_btnFiltrar_clicked();
	void on_btnCerrar_clicked();

private:
	Ui::ReporteWindow *ui;

	//Ejecuta una funcion de reportes.cpp que imprime con cout,
	//captura esa salida y la muestra en txtResultado, sin
	//modificar reportes.cpp/.h.
	void ejecutarYMostrar(const std::function<void()> &funcionReporte);
};

#endif
