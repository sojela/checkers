#ifndef CHECKERSLOGIC_H
#define CHECKERSLOGIC_H

#include <vector>

enum {empty, player1Piece, player2Piece, player1KingPiece, player2KingPiece};

class CheckersLogic {
public:
    CheckersLogic(int numberOfSquaresInBoard);
    bool isValid(const std::pair<int, int>& start, const std::pair<int, int>& destination) const;
    bool canCapture(const std::pair<int, int>& pos) const;
    bool canMoveWithoutCapture(const std::pair<int, int>& pos) const;
    bool isMoveable(const std::pair<int, int>& piecePos) const;
    void setBoard(const std::vector<std::vector<int>>& newBoard);
    bool canCaptureForwards(const std::pair<int, int>& start, const std::pair<int, int>& destination) const;
    bool canCaptureBackwards(const std::pair<int, int>& start, const std::pair<int, int>& destination) const;
    bool canMoveForwardsWithoutCapture(const std::pair<int, int>& start, const std::pair<int, int>& destination) const;
    bool canMoveBackwardWithoutCapture(const std::pair<int, int>& start, const std::pair<int, int>& destination) const;
    bool kinging(const std::pair<int, int>& pos);
    bool isCurrentPlayersPiece(const std::pair<int, int>& pos) const;
    bool movePiece(const std::pair<int, int>& destination);
    int gameOver() const;
    void endTurn();
    void removeCapturedPiece(const std::pair<int, int>& start, const std::pair<int, int>& end);
    void resetBoard();
    bool selectPiece(const std::pair<int, int>& pos);

    const int number_of_squares_in_board;
    std::vector<std::vector<int>> board;
    std::pair<int, int> selectedPiece;
    std::pair<int, int> captured;
    bool pieceSelected;
    bool player1Turn;
    bool hasCapturedThisTurn;
};

#endif // CHECKERSLOGIC_H
