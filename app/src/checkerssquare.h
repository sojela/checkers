#ifndef CHECKERSSQUARE_H
#define CHECKERSSQUARE_H

#include <QGraphicsRectItem>

class CheckersSquare : public QGraphicsRectItem {
public:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
};

#endif // CHECKERSSQUARE_H
