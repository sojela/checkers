#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QColor>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent* event);

private:
    Ui::MainWindow *ui;

    void drawBoard();
    void drawPieces();

    const QColor lightSquare;
    const QColor darkSquare;
    const QColor p1Colour;
    const QColor p2Colour;
    QVector<QVector<std::pair<QRect, int>>> board;
    QSize windowSize;
};
#endif // MAINWINDOW_H
