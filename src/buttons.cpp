#include "buttons.h"
#include "checkers.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

extern Checkers* MainWindow;

PlayButton::PlayButton() {
    this->setPixmap(QPixmap(":/images/play.png"));
}

void PlayButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() != Qt::LeftButton)
        return;

    MainWindow->startNewGame();

    QGraphicsItem::mousePressEvent(event);
}

InfoButton::InfoButton() {
    this->setPixmap(QPixmap(":/images/info.png"));
}

void InfoButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() != Qt::LeftButton)
        return;

    MainWindow->displayCredits();

    QGraphicsItem::mousePressEvent(event);
}
