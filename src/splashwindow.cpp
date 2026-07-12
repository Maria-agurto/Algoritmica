/*widgets usados de splash.ui: lblLogo, lblTitulo, lblCargando.
Se muestra unos segundos al iniciar la aplicacion y luego emite splashFinalizado()
para que main.cpp muestre la MainWindow.*/

#include "../include/splashwindow.h"
#include "ui_splash.h"
#include <QTimer>

SplashWindow::SplashWindow(QWidget *parent):QWidget(parent), ui(new Ui::SplashWindow){
	ui->setupUi(this);
	setWindowFlag(Qt::FramelessWindowHint);

	QTimer::singleShot(2500, this, [this](){
		emit splashFinalizado();
		close();
	});
}

SplashWindow::~SplashWindow(){
	delete ui;
}
