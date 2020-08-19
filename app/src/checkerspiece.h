#ifndef CHECKERSPIECE_H
#define CHECKERSPIECE_H

#include <QGraphicsPixmapItem>

class CheckersPiece : public QGraphicsPixmapItem {
public:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    int typeOfPiece;
};

#endif // CHECKERSPIECE_H
