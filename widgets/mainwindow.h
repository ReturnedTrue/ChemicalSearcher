#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../lib/pubchemclient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

	public:
		MainWindow();
		~MainWindow();

		void goButtonClicked(bool checked);

	private:
		Ui::MainWindow *ui;
		PubChemClient chemClient;

};
#endif // MAINWINDOW_H
