#include "checkerssquare.h"
#include "checkers.h"

#include <QGraphicsSceneMouseEvent>

extern Checkers* w;

void CheckersSquare::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() != Qt::LeftButton)
        return;

    w->movePiece(this->boundingRect().center());
}
