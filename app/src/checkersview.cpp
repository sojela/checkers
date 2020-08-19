#include "checkersview.h"

#include "checkers.h"

#include <QResizeEvent>

extern Checkers* MainWindow;

void CheckersView::resizeEvent(QResizeEvent* event) {
    if(scene()) {
        scene()->setSceneRect(0, 0, event->size().width(), event->size().height());

        if(MainWindow)
            MainWindow->update();
    }

    QGraphicsView::resizeEvent(event);
}
