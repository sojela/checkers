#ifndef BUTTONS_H
#define BUTTONS_H

#include <QGraphicsPixmapItem>

class PlayButton : public QGraphicsPixmapItem {
public:
    PlayButton();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
};

class InfoButton : public QGraphicsPixmapItem {
public:
    InfoButton();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
};

#endif // BUTTONS_H
