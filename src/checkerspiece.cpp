#include "checkerspiece.h"
#include "checkers.h"

#include <QGraphicsSceneMouseEvent>

extern Checkers* MainWindow;

void CheckersPiece::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() != Qt::LeftButton)
        return;

    MainWindow->selectPiece(QPointF(this->x(), this->y()));
}
