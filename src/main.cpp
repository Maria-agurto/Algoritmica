#include <QApplication>
#include <QFile>
#include <QObject>
#include "../include/mainwindow.h"
#include "../include/splashwindow.h"

int main(int argc, char *argv[]){
	QApplication app(argc, argv);

	//Carga el estilo global de la aplicacion (resources/estilos/estilo.qss),
	//registrado en recursos.qrc bajo el prefijo ":/estilos".
	QFile archivoEstilo(":/estilos/estilo.qss");
	if(archivoEstilo.open(QFile::ReadOnly | QFile::Text)){
		QString estilo=QLatin1String(archivoEstilo.readAll());
		app.setStyleSheet(estilo);
		archivoEstilo.close();
	}

	MainWindow *ventanaPrincipal=new MainWindow();

	SplashWindow *splash=new SplashWindow();
	QObject::connect(splash, &SplashWindow::splashFinalizado, [ventanaPrincipal](){
		ventanaPrincipal->show();
	});
	splash->show();

	int resultado=app.exec();

	delete ventanaPrincipal;
	return resultado;

