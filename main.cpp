#include "widgets/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;

	window.setWindowTitle("Chemical Searcher");
    window.show();

    return app.exec();
}
