#ifndef CHECKERSAI_H
#define CHECKERSAI_H

#include "checkerspiece.h"
#include "checkerssquare.h"

#include <QVector>
#include <utility>

class CheckersAI {
public:
    std::pair<std::pair<int, int>, std::pair<int, int>> calculateMoveVeryEasy(const QVector<QVector<std::pair<std::shared_ptr<CheckersSquare>, std::shared_ptr<CheckersPiece>>>>& board);
};

#endif // CHECKERSAI_H