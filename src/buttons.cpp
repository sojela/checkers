#include "buttons.h"
#include "checkers.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

extern Checkers* w;

PlayButton::PlayButton() {
    this->setPixmap(QPixmap(":/images/play.png"));
}

void PlayButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() != Qt::LeftButton)
        return;

    w->startNewGame();

    QGraphicsItem::mousePressEvent(event);
}

InfoButton::InfoButton() {
    this->setPixmap(QPixmap(":/images/info.png"));
}

void InfoButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() != Qt::LeftButton)
        return;

    w->displayCredits();

    QGraphicsItem::mousePressEvent(event);
}
