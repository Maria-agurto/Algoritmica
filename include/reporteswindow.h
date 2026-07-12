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

	//Ejecuta una funcion de reportes.cpp que imprime con cout,
	//captura esa salida y la muestra en txtResultado, sin
	//modificar reportes.cpp/.h.
	void ejecutarYMostrar(const std::function<void()> &funcionReporte);

	//Llena tablaEventos reutilizando leerTodosEventos() de
	//archivos.h (Maria), sin duplicar logica de lectura.
	//Si se le pasa 'eventosOrdenados' no vacio, llena la tabla
	//con ese orden en vez de releer el archivo (para que la
	//tabla refleje el mismo orden que ordenarEventos() imprime
	//en txtResultado, sin tocar reportes.cpp/.h).
	void cargarTablaEventos(const std::vector<Evento> &eventosOrdenados={});
};

#endif

