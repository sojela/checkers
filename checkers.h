#ifndef CHECKERS_H
#define CHECKERS_H

#include "checkerssquare.h"
#include "checkersview.h"
#include "checkerspiece.h"

#include <QMainWindow>
#include <QVector>
#include <QGraphicsScene>


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
    void updateBoard();

private:
    std::pair<int, int> findPiece(QPointF center);
    bool isValid(std::pair<int, int> start, std::pair<int, int> destination);
    bool isCurrentPlayersPiece(std::pair<int, int> pos);
    bool moveForwards(std::pair<int, int> start, std::pair<int, int> destination);
    bool moveBackwards(std::pair<int, int> start, std::pair<int, int> destination);
    bool captureForwards(std::pair<int, int> start, std::pair<int, int> destination);
    bool captureBackwards(std::pair<int, int> start, std::pair<int, int> destination);
    bool canCapture(std::pair<int, int> pos);

    Ui::Checkers *ui;
    const int board_length;
    const QColor light_square;
    const QColor dark_square;
    const QColor dark_square_highlight;
    const QColor player_1_colour_regular;
    const QColor player_1_colour_king;
    const QColor player_2_colour_regular;
    const QColor player_2_colour_king;
    const qreal squareZHeight;
    const qreal pieceZHeight;
    QVector<QVector<std::pair<std::shared_ptr<CheckersSquare>, std::shared_ptr<CheckersPiece>>>> board;
    std::pair<int, int> captured;
    std::pair<int, int> selectedPiece;
    bool pieceSelected;
    bool player1Turn;
    bool hasCapturedThisTurn;
    QGraphicsScene scene;
    CheckersView view;
};
#endif // CHECKERS_H
