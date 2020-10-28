#ifndef CHECKERSLOGIC_H
#define CHECKERSLOGIC_H

#include "types.h"

#include <vector>

class CheckersLogic {
public:
    CheckersLogic();
    bool isValidMove(const Coords& start, const Coords& destination) const;
    bool isMoveable(const Coords& piecePos) const;
    bool canCapture(const Coords& pos) const;
    bool isCurrentPlayersPiece(const Coords& pos) const;
    void movePiece(const Coords& destination);
    GameState getGameState() const;
    void resetBoard();
    bool selectPiece(const Coords& pos);
    static Coords movePos(const Coords& start, const Direction& direction, int step = 1);

    const int number_of_squares_in_board;
    std::vector<std::vector<int>> board;
    Coords selectedPiece;
    bool pieceSelected;
    bool player1Turn;
    bool hasCapturedThisTurn;

private:
    void removeCapturedPiece(const Coords& start, const Coords& end);
    bool canMoveWithoutCapture(const Coords& pos) const;
    bool canCaptureForwards(const Coords& start, const Coords& destination) const;
    bool canCaptureBackwards(const Coords& start, const Coords& destination) const;
    bool canMoveForwardsWithoutCapture(const Coords& start, const Coords& destination) const;
    bool canMoveBackwardWithoutCapture(const Coords& start, const Coords& destination) const;
    bool canMove(const Coords& pos, unsigned int step) const;
    void endTurn();
    void kinging(const Coords& pos);

    Coords captured;
};

#endif // CHECKERSLOGIC_H
