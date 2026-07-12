#include <QApplication>
#include <QCoreApplication>
#include <QFile>
#include <QObject>
#include <QDir>
#include "../include/mainwindow.h"
#include "../include/splashwindow.h"

int main(int argc, char *argv[]){
	QApplication app(argc, argv);

	//CORRECCION: las rutas "data/eventos.dat", "data/participantes.dat", etc.
	//son relativas al directorio de trabajo (cwd) del proceso. Sin esta linea,
	//el cwd dependia de como se lanzara el ejecutable: al correr desde Qt
	//Creator, por defecto es la carpeta de compilacion (shadow build), NO la
	//raiz del proyecto donde vive la carpeta data/ que se revisa a mano. Eso
	//hacia parecer que los registros "no se guardaban en ningun lado", cuando
	//en realidad (si la escritura no fallaba por el otro bug de directorio
	//faltante) se guardaban en una carpeta data/ distinta junto al binario.
	//Fijar el cwd a la carpeta del ejecutable hace el comportamiento
	//predecible: siempre se lee/escribe "<carpeta del .exe>/data/...".
	QDir::setCurrent(QCoreApplication::applicationDirPath());

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
}

