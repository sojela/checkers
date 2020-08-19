#include "checkers.h"

#include <QApplication>

Checkers* MainWindow;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow = new Checkers;
    MainWindow->setWindowTitle("Checkers");
    MainWindow->show();
    MainWindow->startNewGame();

    return a.exec();
}
