#ifndef CHECKERSPIECE_H
#define CHECKERSPIECE_H

#include <QGraphicsPixmapItem>

enum {player1Piece, player2Piece, player1KingPiece, player2KingPiece};

class CheckersPiece : public QGraphicsPixmapItem {
public:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    int typeOfPiece;
};

#endif // CHECKERSPIECE_H
