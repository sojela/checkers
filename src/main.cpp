#include "checkers.h"

#include <QApplication>

Checkers* w;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    w = new Checkers;
    w->setWindowTitle("Checkers");
    w->show();

    return a.exec();
}
