#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui{
class MainWindow;
}

class MainWindow : public QMainWindow{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent=nullptr);
	~MainWindow();

private slots:
	void on_btnRegistrarEvento_clicked();
	void on_btnEditarEvento_clicked();
	void on_btnRegistrarParticipante_clicked();
	void on_btnRegistrarOrganizador_clicked();
	void on_btnInscripciones_clicked();
	void on_btnReportes_clicked();
	void on_btnAcercaDe_clicked();
	void on_btnSalir_clicked();

private:
	Ui::MainWindow *ui;
};

#endif
