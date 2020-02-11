#ifndef CHECKERS_H
#define CHECKERS_H

#include "checkerssquare.h"
#include "checkersview.h"
#include "checkerspiece.h"

#include <QMainWindow>
#include <QVector>
#include <QGraphicsScene>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class Checkers; }
QT_END_NAMESPACE

class Checkers : public QMainWindow
{
    Q_OBJECT

public:
    Checkers(QWidget *parent = nullptr);
    ~Checkers();
    void selectPiece(QPointF center);
    void movePiece(QPointF center);

private slots:
    void drawBoard();

private:
    Ui::Checkers *ui;
    const int board_length;
    const QColor light_square;
    const QColor dark_square;
    const QColor player_1_colour;
    const QColor player_2_colour;
    const qreal squareZHeight;
    const qreal pieceZHeight;
    QVector<QVector<std::pair<std::shared_ptr<CheckersSquare>, std::shared_ptr<CheckersPiece>>>> board;
    std::pair<int, int> selectedPiece;
    QGraphicsScene scene;
    CheckersView view;
    QTimer* timer;
};
#endif // CHECKERS_H
