#include "checkerspiece.h"
#include "checkers.h"

#include <QGraphicsSceneMouseEvent>

extern Checkers* w;

void CheckersPiece::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() != Qt::LeftButton)
        return;

    w->selectPiece(QPointF(this->x(), this->y()));
}
