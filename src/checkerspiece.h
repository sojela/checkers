#ifndef CHECKERSPIECE_H
#define CHECKERSPIECE_H

#include <QGraphicsPixmapItem>

enum {player1Piece = 1, player2Piece = 2, player1KingPiece = 3, player2KingPiece = 4};

class CheckersPiece : public QGraphicsPixmapItem {
public:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    int typeOfPiece;
};

#endif // CHECKERSPIECE_H
