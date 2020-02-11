#include "checkersview.h"

#include <QResizeEvent>

void CheckersView::resizeEvent(QResizeEvent* event) {
    if(scene())
        scene()->setSceneRect(0, 0, event->size().width(), event->size().height());

    QGraphicsView::resizeEvent(event);
}
