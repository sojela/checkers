#ifndef CHECKERSLOGIC_H
#define CHECKERSLOGIC_H

#include "types.h"

#include <vector>
#include <memory>

class CheckersLogic {
public:
    CheckersLogic();
    ~CheckersLogic();
    CheckersLogic(const CheckersLogic& state);
    void movePiece(const Coords& destination);
    GameState getGameState() const;
    void resetBoard();
    bool selectPiece(const Coords& pos);
    static Coords movePos(const Coords& start, const Direction& direction, int step = 1);
    const std::vector<std::vector<int>>& getBoard() const;
    const Coords& getSelectedPiece() const;
    bool isPieceSelected() const;
    bool hasCapturedThisTurn() const;
    bool isValidMove(const Coords& start, const Coords& destination) const;
    bool isPlayer1Turn() const;
    bool isMoveable(const Coords& piecePos) const;

    static const int number_of_squares_in_board = 8;
private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

#endif // CHECKERSLOGIC_H
