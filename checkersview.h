#ifndef CHECKERSVIEW_H
#define CHECKERSVIEW_H

#include <QGraphicsView>

class CheckersView : public QGraphicsView {
public:
    CheckersView();
protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // CHECKERSVIEW_H
