#ifndef CHECKERSAI_H
#define CHECKERSAI_H

#include "checkerspiece.h"
#include "checkerssquare.h"
#include "checkerslogic.h"

#include <QVector>
#include <utility>

class CheckersAI {
public:
    std::pair<std::pair<int, int>, std::pair<int, int>> calculateMoveVeryEasy(CheckersLogic currentGameState);
};

#endif // CHECKERSAI_H
