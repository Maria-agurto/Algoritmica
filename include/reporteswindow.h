#ifndef REPORTESWINDOW_H
#define REPORTESWINDOW_H

#include <QWidget>
#include <vector>
#include "evento.h"

// Namespace generado por uic a partir de ui/reportes.ui
namespace Ui {
class ReportesWindow;
}

// ReportesWindow
// Responsable: Adrian - Modulo de Reportes (Etapa 4: interfaz Qt)
//
// Esta clase es el "controlador" que conecta la ventana reportes.ui con
// la logica de negocio de reportes.cpp/reportes.h. No contiene logica de
// negocio propia: solo toma lo que el usuario escribe/selecciona en la
// UI, llama a las funciones "de datos" de reportes.h, y pinta el
// resultado en la tabla o en el cuadro de texto. Toda la logica real
// (lectura de archivos .dat, calculos, ordenamiento, filtrado) vive en
// reportes.cpp, que sigue siendo C++ puro sin ningun include de Qt.
class ReportesWindow : public QWidget {
    Q_OBJECT

public:
    explicit ReportesWindow(QWidget *parent = nullptr);
    ~ReportesWindow();

private slots:
    // Cada slot corresponde a uno de los botones de reportes.ui y llama
    // a la funcion de datos equivalente en reportes.h
    void onReporteGeneral();
    void onEventoMayorAsistencia();
    void onEventosDisponibles();
    void onEventosLlenos();
    void onPorcentajeOcupacion();
    void onOrdenarEventos();
    void onFiltrarEventos();
    void onLimpiarFiltro();

private:
    Ui::ReportesWindow *ui;

    // Pinta una lista de eventos en tablaEventos (columnas 0 a 6).
    // Si mostrarOcupacion es true, tambien calcula y llena la columna
    // "Ocupacion" (columna 7).
    void mostrarEventosEnTabla(const std::vector<Evento> &eventos, bool mostrarOcupacion = false);

    // Limpia la tabla y el cuadro de texto de resultados
    void limpiarVista();
};

#endif // REPORTESWINDOW_H
