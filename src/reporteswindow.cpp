#include "reporteswindow.h"
#include "ui_reportes.h"
#include "reportes.h"
#include "validaciones.h"

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QString>

ReportesWindow::ReportesWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::ReportesWindow) {
    ui->setupUi(this);

    // Conexion de cada boton de reportes.ui con su slot correspondiente
    connect(ui->btnReporteGeneral, &QPushButton::clicked, this, &ReportesWindow::onReporteGeneral);
    connect(ui->btnEventoMayorAsistencia, &QPushButton::clicked, this, &ReportesWindow::onEventoMayorAsistencia);
    connect(ui->btnEventosDisponibles, &QPushButton::clicked, this, &ReportesWindow::onEventosDisponibles);
    connect(ui->btnEventosLlenos, &QPushButton::clicked, this, &ReportesWindow::onEventosLlenos);
    connect(ui->btnPorcentajeOcupacion, &QPushButton::clicked, this, &ReportesWindow::onPorcentajeOcupacion);
    connect(ui->btnOrdenar, &QPushButton::clicked, this, &ReportesWindow::onOrdenarEventos);
    connect(ui->btnFiltrar, &QPushButton::clicked, this, &ReportesWindow::onFiltrarEventos);
    connect(ui->btnLimpiarFiltro, &QPushButton::clicked, this, &ReportesWindow::onLimpiarFiltro);

    limpiarVista();
}

ReportesWindow::~ReportesWindow() {
    delete ui;
}

// -----------------------------------------------------------------------
// Helpers privados
// -----------------------------------------------------------------------

void ReportesWindow::limpiarVista() {
    ui->tablaEventos->setRowCount(0);
    ui->txtResultado->clear();
}

void ReportesWindow::mostrarEventosEnTabla(const std::vector<Evento> &eventos, bool mostrarOcupacion) {
    ui->tablaEventos->setRowCount(static_cast<int>(eventos.size()));

    for (int fila = 0; fila < static_cast<int>(eventos.size()); ++fila) {
        const Evento &e = eventos[fila];

        ui->tablaEventos->setItem(fila, 0, new QTableWidgetItem(QString::number(e.idEvento)));
        ui->tablaEventos->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(e.titulo)));
        ui->tablaEventos->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(e.categoria)));
        ui->tablaEventos->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(e.fecha)));
        ui->tablaEventos->setItem(fila, 4, new QTableWidgetItem(QString::number(e.capacidad)));
        ui->tablaEventos->setItem(fila, 5, new QTableWidgetItem(QString::number(e.inscritos)));
        ui->tablaEventos->setItem(fila, 6, new QTableWidgetItem(QString::fromStdString(e.estado)));

        if (mostrarOcupacion) {
            double porcentaje = (e.capacidad > 0)
                                     ? (static_cast<double>(e.inscritos) / e.capacidad) * 100.0
                                     : 0.0;
            QString texto = QString::number(porcentaje, 'f', 2) + "%";
            ui->tablaEventos->setItem(fila, 7, new QTableWidgetItem(texto));
        } else {
            ui->tablaEventos->setItem(fila, 7, new QTableWidgetItem(""));
        }
    }

    ui->tablaEventos->resizeColumnsToContents();
}

// -----------------------------------------------------------------------
// Slots - cada uno llama a la funcion de datos correspondiente de
// reportes.h (definida en reportes.cpp)
// -----------------------------------------------------------------------

void ReportesWindow::onReporteGeneral() {
    limpiarVista();
    mostrarEventosEnTabla(obtenerTodosLosEventos());
    ui->txtResultado->setPlainText(QString::fromStdString(obtenerReporteGeneralTexto()));
}

void ReportesWindow::onEventoMayorAsistencia() {
    limpiarVista();
    Evento mayor = obtenerEventoMayorAsistenciaData();

    if (mayor.idEvento == -1) {
        ui->txtResultado->setPlainText("No hay eventos registrados en el sistema.");
        return;
    }

    mostrarEventosEnTabla({mayor});
    QString texto = QString("Evento con mayor asistencia:\n%1 (ID %2) - %3/%4 inscritos - Organizador: %5")
                         .arg(QString::fromStdString(mayor.titulo))
                         .arg(mayor.idEvento)
                         .arg(mayor.inscritos)
                         .arg(mayor.capacidad)
                         .arg(QString::fromStdString(mayor.organizador));
    ui->txtResultado->setPlainText(texto);
}

void ReportesWindow::onEventosDisponibles() {
    limpiarVista();
    std::vector<Evento> eventos = obtenerEventosDisponiblesData();
    mostrarEventosEnTabla(eventos);
    ui->txtResultado->setPlainText(
        QString("Eventos disponibles encontrados: %1").arg(eventos.size()));
}

void ReportesWindow::onEventosLlenos() {
    limpiarVista();
    std::vector<Evento> eventos = obtenerEventosLlenosData();
    mostrarEventosEnTabla(eventos);
    ui->txtResultado->setPlainText(
        QString("Eventos llenos encontrados: %1").arg(eventos.size()));
}

void ReportesWindow::onPorcentajeOcupacion() {
    limpiarVista();
    std::vector<std::pair<Evento, double>> datos = obtenerPorcentajeOcupacionData();

    std::vector<Evento> eventos;
    eventos.reserve(datos.size());
    for (const auto &par : datos) eventos.push_back(par.first);

    mostrarEventosEnTabla(eventos, /*mostrarOcupacion=*/true);
    ui->txtResultado->setPlainText("Columna 'Ocupacion' calculada para cada evento.");
}

void ReportesWindow::onOrdenarEventos() {
    limpiarVista();

    // Los QComboBox de reportes.ui ya restringen las opciones validas,
    // por lo que el indice (0..N-1) se traduce directo al criterio
    // esperado por obtenerEventosOrdenadosData() (1..N)
    int criterio = ui->cmbCriterioOrden->currentIndex() + 1; // 1=Titulo 2=Fecha 3=Capacidad 4=Inscritos
    bool ascendente = (ui->cmbOrdenDireccion->currentIndex() == 0); // 0=Ascendente 1=Descendente

    std::vector<Evento> eventos = obtenerEventosOrdenadosData(criterio, ascendente);
    mostrarEventosEnTabla(eventos);

    ui->txtResultado->setPlainText(
        QString("Eventos ordenados por '%1' (%2).")
            .arg(ui->cmbCriterioOrden->currentText())
            .arg(ui->cmbOrdenDireccion->currentText()));
}

void ReportesWindow::onFiltrarEventos() {
    limpiarVista();

    int criterio = ui->cmbCriterioFiltro->currentIndex() + 1; // 1=Categoria 2=Estado 3=Fecha
    QString valorQt = ui->txtValorFiltro->text().trimmed();

    if (valorQt.isEmpty()) {
        QMessageBox::warning(this, "Filtrar eventos", "Debe ingresar un valor para filtrar.");
        return;
    }

    std::string valor = valorQt.toStdString();

    if (criterio == 3 && !validarFecha(valor)) {
        QMessageBox::warning(this, "Filtrar eventos",
                              "La fecha ingresada no tiene un formato valido (dd/mm/yyyy).");
        return;
    }

    std::vector<Evento> eventos = obtenerEventosFiltradosData(criterio, valor);
    mostrarEventosEnTabla(eventos);

    if (eventos.empty()) {
        ui->txtResultado->setPlainText("No se encontraron eventos que coincidan con el filtro.");
    } else {
        ui->txtResultado->setPlainText(
            QString("Coincidencias encontradas: %1").arg(eventos.size()));
    }
}

void ReportesWindow::onLimpiarFiltro() {
    ui->txtValorFiltro->clear();
    limpiarVista();
}
