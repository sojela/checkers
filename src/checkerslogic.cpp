#include "checkerslogic.h"

#include <cstdlib>

CheckersLogic::CheckersLogic(int numberOfSquaresInBoard)
        : number_of_squares_in_board(numberOfSquaresInBoard)
        , selectedPiece(-1, -1)
{
    // set board dimensions
    board.resize(number_of_squares_in_board);
    for(int i = 0; i < number_of_squares_in_board; ++i)
        board[i].resize(number_of_squares_in_board);
}

void CheckersLogic::removeCapturedPiece(const std::pair<int, int>& start, const std::pair<int, int>& end) {
    std::pair<int, int> captured;

    if(end.first > start.first)
        captured.first = start.first + 1;
    else
        captured.first = start.first - 1;

    if(end.second > start.second)
        captured.second = start.second + 1;
    else
        captured.second = start.second - 1;

    board[captured.first][captured.second] = empty;
}

void CheckersLogic::resetBoard() {
    for(int i = 0; i < number_of_squares_in_board; ++i) {
        for(int j = 0; j < number_of_squares_in_board; ++j) {
            if(j < 3 && (i + j) % 2)
                board[i][j] = player2Piece;
            else if(j > (number_of_squares_in_board - 4) && (i + j) % 2)
                board[i][j] = player1Piece;
            else
                board[i][j] = empty;
        }
    }

    pieceSelected = false;
    player1Turn = true;
    hasCapturedThisTurn = false;
}

void CheckersLogic::endTurn() {
    hasCapturedThisTurn = false;

    if(player1Turn)
        player1Turn = false;
    else
        player1Turn = true;
}

bool CheckersLogic::isValid(const std::pair<int, int>& start, const std::pair<int, int> &destination) const {
    if(board[start.first][start.second] == empty)
        return false;

    if(board[destination.first][destination.second] != empty)
        return false;

    if(!isCurrentPlayersPiece(start))
        return false;

    if(canMoveForwardsWithoutCapture(start, destination)
    || canMoveBackwardWithoutCapture(start, destination)
    || canCaptureForwards(start, destination)
    || canCaptureBackwards(start, destination))
        return true;

    return false;
}

bool CheckersLogic::canCapture(const std::pair<int, int> &pos) const {
    std::pair<int, int> upLeft {pos.first - 2, pos.second - 2};
    std::pair<int, int> upRight {pos.first + 2, pos.second - 2};
    std::pair<int, int> downLeft {pos.first - 2, pos.second + 2};
    std::pair<int, int> downRight {pos.first + 2, pos.second + 2};

    if(upLeft.first >= 0 && upLeft.second >= 0)
        if(isValid(pos, upLeft)) return true;

    if(upRight.first < number_of_squares_in_board && upRight.second >= 0)
        if(isValid(pos, upRight)) return true;

    if(downLeft.first >= 0 && downLeft.second < number_of_squares_in_board)
        if(isValid(pos, downLeft)) return true;

    if(downRight.first < number_of_squares_in_board && downRight.second < number_of_squares_in_board)
        if(isValid(pos, downRight)) return true;

    return false;
}

bool CheckersLogic::canMoveWithoutCapture(const std::pair<int, int>& pos) const {
    std::pair<int, int> upLeft {pos.first - 1, pos.second - 1};
    std::pair<int, int> upRight {pos.first + 1, pos.second - 1};
    std::pair<int, int> downLeft {pos.first - 1, pos.second + 1};
    std::pair<int, int> downRight {pos.first + 1, pos.second + 1};

    if(upLeft.first >= 0 && upLeft.second >= 0)
        if(isValid(pos, upLeft)) return true;

    if(upRight.first < number_of_squares_in_board && upRight.second >= 0)
        if(isValid(pos, upRight)) return true;

    if(downLeft.first >= 0 && downLeft.second < number_of_squares_in_board)
        if(isValid(pos, downLeft)) return true;

    if(downRight.first < number_of_squares_in_board && downRight.second < number_of_squares_in_board)
        if(isValid(pos, downRight)) return true;

    return false;

}

bool CheckersLogic::isMoveable(const std::pair<int, int>& piecePos) const {
    if(canCapture(piecePos))
        return true;

    if(canMoveWithoutCapture(piecePos))
        return true;

    return false;
}

void CheckersLogic::setBoard(const std::vector<std::vector<int> > &newBoard) {
    for(int i = 0; i < number_of_squares_in_board; ++i) {
        for(int j = 0; j < number_of_squares_in_board; ++j) {
            board[i][j] = newBoard[i][j];
        }
    }
}

bool CheckersLogic::canCaptureForwards(const std::pair<int, int> &start, const std::pair<int, int> &destination) const {
    if(player1Turn) {
        if(start.second - destination.second == 2 && abs(destination.first - start.first) == 2) {
            std::pair<int, int> enemyPos;
            if(destination.first > start.first)
                enemyPos = {start.first + 1, start.second - 1};
            else
                enemyPos = {start.first - 1, start.second - 1};

            if(board[enemyPos.first][enemyPos.second] == empty)
                return false;

            if(!isCurrentPlayersPiece({enemyPos.first, enemyPos.second})) {
                return true;
            }
        }
    } else {
        if(destination.second - start.second == 2 && abs(destination.first - start.first) == 2) {
            std::pair<int, int> enemyPos;
            if(destination.first > start.first)
                enemyPos = {start.first + 1, start.second + 1};
            else
                enemyPos = {start.first - 1, start.second + 1};

            if(board[enemyPos.first][enemyPos.second] == empty)
                return false;

            if(!isCurrentPlayersPiece({enemyPos.first, enemyPos.second})) {
                return true;
            }
        }
    }

    return false;
}

bool CheckersLogic::canCaptureBackwards(const std::pair<int, int> &start, const std::pair<int, int> &destination) const {
    if(player1Turn) {
        if(board[start.first][start.second] == player1KingPiece) {
            if(destination.second - start.second == 2 && abs(destination.first - start.first) == 2) {
                std::pair<int, int> enemyPos;
                if(destination.first > start.first)
                    enemyPos = {start.first + 1, start.second + 1};
                else
                    enemyPos = {start.first - 1, start.second + 1};

                if(board[enemyPos.first][enemyPos.second] == empty)
                    return false;

                if(!isCurrentPlayersPiece({enemyPos.first, enemyPos.second})) {
                    return true;
                }
            }
        }
    } else {
        if(board[start.first][start.second] == player2KingPiece) {
            if(start.second - destination.second == 2 && abs(destination.first - start.first) == 2) {
                std::pair<int, int> enemyPos;
                if(destination.first > start.first)
                    enemyPos = {start.first + 1, start.second - 1};
                else
                    enemyPos = {start.first - 1, start.second - 1};

                if(board[enemyPos.first][enemyPos.second] == empty)
                    return false;

                if(!isCurrentPlayersPiece({enemyPos.first, enemyPos.second})) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool CheckersLogic::isCurrentPlayersPiece(const std::pair<int, int> &pos) const {
    if(board[pos.first][pos.second] == player1Piece ||
            board[pos.first][pos.second] == player1KingPiece) {
        if(player1Turn)
            return true;
        else
            return false;
    } else {
        if(player1Turn)
            return false;
        else
            return true;
    }
}

bool CheckersLogic::movePiece(const std::pair<int, int>& destination) {
    if(!pieceSelected)
        return false;

    if(isValid(selectedPiece, destination)) {
        board[destination.first][destination.second] = board[selectedPiece.first][selectedPiece.second];
        board[selectedPiece.first][selectedPiece.second] = empty;

        if(abs(destination.first - selectedPiece.first) == 2) {
            removeCapturedPiece(selectedPiece, destination);
            hasCapturedThisTurn = true;
        }

        if(hasCapturedThisTurn && canCapture(destination)) {
            selectedPiece = destination;
        } else {
            pieceSelected = false;
        }

        return true;
    }

    return false;
}

int CheckersLogic::gameOver() const {
    int countPlayer1Pieces = 0;
    int countPlayer2Pieces = 0;
    int countPlayer1MoveablePieces = 0;
    int countPlayer2MoveablePieces = 0;

    for(int i = 0; i < number_of_squares_in_board; ++i) {
        for(int j = 0; j < number_of_squares_in_board; ++j) {
            if(board[i][j] == player1Piece
                || board[i][j] == player1KingPiece) {
                ++countPlayer1Pieces;

                if(isMoveable({i, j}))
                ++countPlayer1MoveablePieces;
            } else if(board[i][j] == player2Piece
                || board[i][j] == player2KingPiece) {
                ++countPlayer2Pieces;

                if(isMoveable({i, j}))
                ++countPlayer2MoveablePieces;
            }
        }
    }

    if(countPlayer1Pieces == 0
            || (countPlayer1MoveablePieces == 0 && player1Turn))
        return 2;
    else if(countPlayer2Pieces == 0
            || (countPlayer2MoveablePieces == 0 && !player1Turn))
        return 1;

    return 0;
}

bool CheckersLogic::canMoveForwardsWithoutCapture(const std::pair<int, int>& start, const std::pair<int, int>& destination) const {
    if(hasCapturedThisTurn)
        return false;

    if(player1Turn) {
        if(start.second - destination.second == 1 && abs(destination.first - start.first) == 1)
            return true;
    } else if(destination.second - start.second == 1 && abs(destination.first - start.first) == 1)
        return true;

    return false;
}

bool CheckersLogic::canMoveBackwardWithoutCapture(const std::pair<int, int> &start, const std::pair<int, int> &destination) const {
    if(hasCapturedThisTurn)
        return false;

    if(player1Turn) {
        if(board[start.first][start.second] == player1KingPiece
            && destination.second - start.second == 1
            && abs(destination.first - start.first) == 1)
            return true;
    } else {
        if(board[start.first][start.second] == player2KingPiece
            && start.second - destination.second == 1
            && abs(destination.first - start.first) == 1)
            return true;
    }

    return false;
}

bool CheckersLogic::kinging(const std::pair<int, int> &pos) {
    if(pos.second == 0 && board[pos.first][pos.second] == player1Piece) {
        board[pos.first][pos.second] = player1KingPiece;
        return true;
    } else if(pos.second == number_of_squares_in_board - 1 && board[pos.first][pos.second] == player2Piece) {
        board[pos.first][pos.second] = player2KingPiece;
        return true;
    }

    return false;
}
