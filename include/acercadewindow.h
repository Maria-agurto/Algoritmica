#ifndef ACERCADEWINDOW_H
#define ACERCADEWINDOW_H

#include <QDialog>

namespace Ui{
class AcercaDeWindow;
}

class AcercaDeWindow : public QDialog{
	Q_OBJECT

public:
	explicit AcercaDeWindow(QWidget *parent=nullptr);
	~AcercaDeWindow();

private slots:
	void on_btnCerrar_clicked();

private:
	Ui::AcercaDeWindow *ui;
};

#endif
