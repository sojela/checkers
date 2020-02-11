#include "checkersview.h"

#include "checkers.h"

#include <QResizeEvent>

extern Checkers* w;

void CheckersView::resizeEvent(QResizeEvent* event) {
    if(scene()) {
        scene()->setSceneRect(0, 0, event->size().width(), event->size().height());
        if(w)
            w->updateBoard();
    }

    QGraphicsView::resizeEvent(event);
}
