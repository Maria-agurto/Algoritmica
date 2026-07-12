#ifndef SPLASHWINDOW_H
#define SPLASHWINDOW_H

#include <QWidget>

namespace Ui{
class SplashWindow;
}

class SplashWindow : public QWidget{
	Q_OBJECT

public:
	explicit SplashWindow(QWidget *parent=nullptr);
	~SplashWindow();

signals:
	void splashFinalizado();

private:
	Ui::SplashWindow *ui;
};

#endif
