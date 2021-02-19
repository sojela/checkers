#ifndef CHECKERSLOGIC_H
#define CHECKERSLOGIC_H

#include "types.h"

#include <memory>

class CheckersLogic {
public:
    static const int number_of_squares_in_board = 8;

    CheckersLogic();
    ~CheckersLogic();
    CheckersLogic(const CheckersLogic& state);
    void movePiece(const Coords& destination);
    GameState getGameState() const;
    void resetBoard();
    bool selectPiece(const Coords& pos);
    static Coords movePos(const Coords& start, const Direction& direction, int step = 1);
    const std::array<std::array<int, number_of_squares_in_board>, number_of_squares_in_board>& getBoard() const;
    const Coords& getSelectedPiece() const;
    bool isPieceSelected() const;
    bool hasCapturedThisTurn() const;
    //assumes valid coordinates
    bool isValidMove(const Coords& start, const Coords& destination) const;
    bool isPlayer1Turn() const;
    bool isMoveable(const Coords& piecePos) const;
    // assume there is a piece at pos
    bool isCurrentPlayersPiece(const Coords& pos) const;
private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

#endif // CHECKERSLOGIC_H
