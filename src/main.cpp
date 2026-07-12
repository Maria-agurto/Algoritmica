#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QWidget ventanaPrueba;
    ventanaPrueba.setWindowTitle("Sistema de Eventos - Modo Desarrollo");
    ventanaPrueba.resize(400, 300);
    ventanaPrueba.show();
    
    return app.exec();
}
