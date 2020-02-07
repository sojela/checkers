#include "checkers.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Checkers w;
    w.setWindowTitle("Checkers");
    w.show();
    return a.exec();
}
