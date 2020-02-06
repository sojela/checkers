#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QColor>
#include <QVector>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    virtual void paintEvent(QPaintEvent* event);
    std::pair<QRect, int>* findSquare(QPoint pos);
    void mouseReleaseEvent(QMouseEvent* event);
    void drawBoard();
    void drawPieces();

    const QColor lightSquare;
    const QColor darkSquare;
    const QColor p1Colour;
    const QColor p2Colour;
    QVector<QVector<std::pair<QRect, int>>> board;
    QSize windowSize;
    std::pair<QRect, int>* selectedPiece;
};
#endif // MAINWINDOW_H
