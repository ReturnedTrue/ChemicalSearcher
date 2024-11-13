#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <chrono>

#include "../include/discord/discord.h"
#include "../include/pubchem/pubchemclient.h"

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

	private:
		void goButtonClicked(bool checked);
		void update();
		bool eventFilter(QObject *object, QEvent *event);

		Ui::MainWindow *ui;
		PubChemClient chemClient;
		discord::Core *discordCore;
		discord::Activity discordActivity;

};
#endif // MAINWINDOW_H
