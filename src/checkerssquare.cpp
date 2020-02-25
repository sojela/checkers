#include "checkerssquare.h"
#include "checkers.h"

#include <QGraphicsSceneMouseEvent>

extern Checkers* MainWindow;

void CheckersSquare::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() != Qt::LeftButton)
        return;

    MainWindow->movePiece(this->boundingRect().center());
}
