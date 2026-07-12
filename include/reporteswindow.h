#ifndef REPORTESWINDOW_H
#define REPORTESWINDOW_H

#include <QWidget>
#include <functional>
#include <vector>
#include "evento.h"

namespace Ui{
class ReportesWindow;
}

class ReportesWindow : public QWidget{
	Q_OBJECT

public:
	explicit ReportesWindow(QWidget *parent=nullptr);
	~ReportesWindow();

private slots:
	void on_btnReporteGeneral_clicked();
	void on_btnEventoMayorAsistencia_clicked();
	void on_btnEventosDisponibles_clicked();
	void on_btnEventosLlenos_clicked();
	void on_btnPorcentajeOcupacion_clicked();
	void on_btnOrdenar_clicked();
	void on_btnFiltrar_clicked();
	void on_btnLimpiarFiltro_clicked();

private:
	Ui::ReportesWindow *ui;

	void ejecutarYMostrar(const std::function<void()> &funcionReporte);

	void cargarTablaEventos(const std::vector<Evento> &eventosOrdenados={});
};

#endif

