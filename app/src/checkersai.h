#ifndef CHECKERSAI_H
#define CHECKERSAI_H

#include "checkerspiece.h"
#include "checkerssquare.h"
#include "checkerslogic.h"

#include <QVector>
#include <utility>
#include <vector>

struct Move {
    Coords start;
    Coords destination;
};

class CheckersAI {
public:
    Move calculateMoveVeryEasy(const CheckersLogic& currentGameState);
private:
    std::vector<Coords> calculateAllMovesForPiece(const Coords& piece);
    int randomNumber(unsigned int max);
};

#endif // CHECKERSAI_H
