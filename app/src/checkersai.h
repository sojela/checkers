#ifndef CHECKERSAI_H
#define CHECKERSAI_H

#include "checkerssquare.h"
#include "checkerslogic.h"

struct Move {
    Coords start;
    Coords destination;
};

class CheckersAI {
public:
    CheckersAI();
    ~CheckersAI();

    Move calculateMoveVeryEasy(const CheckersLogic& currentGameState);

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

#endif // CHECKERSAI_H
