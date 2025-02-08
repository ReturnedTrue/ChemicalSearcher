#include "widgets/mainwindow.h"

#include <QApplication>

constexpr QLatin1String PROGRAM_NAME("Chemical Searcher");

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;

    window.setWindowTitle(PROGRAM_NAME);
    window.show();

    return app.exec();
}
