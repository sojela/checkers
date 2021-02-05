#include "checkerslogic.h"

#include <cstdlib>

CheckersLogic::CheckersLogic()
        : number_of_squares_in_board(8)
{
    // set board dimensions
    board.resize(number_of_squares_in_board);
    for(int i = 0; i < number_of_squares_in_board; ++i)
        board[i].resize(number_of_squares_in_board);
}

void CheckersLogic::removeCapturedPiece(const Coords& start, const Coords& end) {
    Coords captured;

    if(end.x > start.x)
        captured.x = start.x + 1;
    else
        captured.x = start.x - 1;

    if(end.y > start.y)
        captured.y = start.y + 1;
    else
        captured.y = start.y - 1;

    board[captured.x][captured.y] = empty;
}

void CheckersLogic::resetBoard() {
    for(int i = 0; i < number_of_squares_in_board; ++i) {
        for(int j = 0; j < number_of_squares_in_board; ++j) {
            bool oddSquare = (i + j) % 2;

            if(oddSquare) {
                bool topOfBoard = j < 3;
                bool bottomOfBoard = j > number_of_squares_in_board - 4;

                if(topOfBoard)
                    board[i][j] = player2Piece;
                else if(bottomOfBoard)
                    board[i][j] = player1Piece;
                else
                    board[i][j] = empty;
            } else
                board[i][j] = empty;
        }
    }

    pieceSelected = false;
    player1Turn = true;
    hasCapturedThisTurn = false;
}

bool CheckersLogic::selectPiece(const Coords& pos) {
    if(isMoveable(pos)) {
        selectedPiece = pos;
        pieceSelected = true;
        return true;
    } else
        return false;
}

void CheckersLogic::endTurn() {
    hasCapturedThisTurn = false;
    pieceSelected = false;

    if(player1Turn)
        player1Turn = false;
    else
        player1Turn = true;
}

// assume valid coordinates
bool CheckersLogic::isValidMove(const Coords& start, const Coords& destination) const {
    if(board[start.x][start.y] == empty)
        return false;

    if(board[destination.x][destination.y] != empty)
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

bool CheckersLogic::canCapture(const Coords& pos) const {
    return canMove(pos, 2);
}

bool CheckersLogic::canMoveWithoutCapture(const Coords& pos) const {
    return canMove(pos, 1);
}

bool CheckersLogic::canMove(const Coords &pos, unsigned int step) const {
    auto upLeft = movePos(pos, upLeftPos, step);
    if(upLeft.x >= 0 && upLeft.y >= 0)
        if(isValidMove(pos, upLeft)) return true;

    auto upRight = movePos(pos, upRightPos, step);
    if(upRight.x < number_of_squares_in_board && upRight.y >= 0)
        if(isValidMove(pos, upRight)) return true;

    auto downLeft = movePos(pos, downLeftPos, step);
    if(downLeft.x >= 0 && downLeft.y < number_of_squares_in_board)
        if(isValidMove(pos, downLeft)) return true;

    auto downRight = movePos(pos, downRightPos, step);
    if(downRight.x < number_of_squares_in_board && downRight.y < number_of_squares_in_board)
        if(isValidMove(pos, downRight)) return true;

    return false;
}

bool CheckersLogic::isMoveable(const Coords& piecePos) const {
    if(canCapture(piecePos))
        return true;

    if(canMoveWithoutCapture(piecePos))
        return true;

    return false;
}

bool CheckersLogic::canCaptureForwards(const Coords& start, const Coords& destination) const {
    if(player1Turn) {
        if(start.y - destination.y == 2 && abs(destination.x - start.x) == 2) {
            Coords enemyPos;

            if(destination.x > start.x)
                enemyPos = {start.x + 1, start.y - 1};
            else
                enemyPos = {start.x - 1, start.y - 1};

            auto& enemy = board[enemyPos.x][enemyPos.y];
            if(enemy == player2Piece
            || enemy == player2KingPiece)
                return true;
        }
    } else {
        if(destination.y - start.y == 2 && abs(destination.x - start.x) == 2) {
            Coords enemyPos;

            if(destination.x > start.x)
                enemyPos = {start.x + 1, start.y + 1};
            else
                enemyPos = {start.x - 1, start.y + 1};

            auto& enemy = board[enemyPos.x][enemyPos.y];
            if(enemy == player1Piece
            || enemy == player1KingPiece)
                return true;
        }
    }

    return false;
}

bool CheckersLogic::canCaptureBackwards(const Coords& start, const Coords& destination) const {
    if(player1Turn) {
        if(board[start.x][start.y] == player1KingPiece) {
            if(destination.y - start.y == 2 && abs(destination.x - start.x) == 2) {
                Coords enemyPos;

                if(destination.x > start.x)
                    enemyPos = {start.x + 1, start.y + 1};
                else
                    enemyPos = {start.x - 1, start.y + 1};

                auto& enemy = board[enemyPos.x][enemyPos.y];
                if(enemy == player2Piece
                || enemy == player2KingPiece)
                    return true;
            }
        }
    } else {
        if(board[start.x][start.y] == player2KingPiece) {
            if(start.y - destination.y == 2 && abs(destination.x - start.x) == 2) {
                Coords enemyPos;

                if(destination.x > start.x)
                    enemyPos = {start.x + 1, start.y - 1};
                else
                    enemyPos = {start.x - 1, start.y - 1};

                auto& enemy = board[enemyPos.x][enemyPos.y];
                if(enemy == player1Piece
                || enemy == player1KingPiece)
                    return true;
            }
        }
    }

    return false;
}

// assume there is a piece at pos
bool CheckersLogic::isCurrentPlayersPiece(const Coords& pos) const {
    if(board[pos.x][pos.y] == player1Piece
    || board[pos.x][pos.y] == player1KingPiece) {
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

void CheckersLogic::movePiece(const Coords& destination) {
    if(!pieceSelected) return;

    if(isValidMove(selectedPiece, destination)) {
        board[destination.x][destination.y] = board[selectedPiece.x][selectedPiece.y];
        board[selectedPiece.x][selectedPiece.y] = empty;

        if(abs(destination.x - selectedPiece.x) == 2) {
            removeCapturedPiece(selectedPiece, destination);
            hasCapturedThisTurn = true;
        }

        if(hasCapturedThisTurn && canCapture(destination))
            selectedPiece = destination;
        else {
            kinging(destination);
            endTurn();
        }
    }
}

GameState CheckersLogic::getGameState() const {
    int countPlayer1Pieces = 0;
    int countPlayer2Pieces = 0;
    int countPlayer1MoveablePieces = 0;
    int countPlayer2MoveablePieces = 0;

    for(int i = 0; i < number_of_squares_in_board; ++i) {
        for(int j = 0; j < number_of_squares_in_board; ++j) {
            bool evenSquare = !((i + j) % 2);
            if(evenSquare) continue;

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
        return player2Wins;
    else if(countPlayer2Pieces == 0
    || (countPlayer2MoveablePieces == 0 && !player1Turn))
        return player1Wins;

    return current;
}

bool CheckersLogic::canMoveForwardsWithoutCapture(const Coords& start, const Coords& destination) const {
    if(hasCapturedThisTurn)
        return false;

    if(player1Turn) {
        if(start.y - destination.y == 1 && abs(destination.x - start.x) == 1)
            return true;
    } else if(destination.y - start.y == 1 && abs(destination.x - start.x) == 1)
        return true;

    return false;
}

bool CheckersLogic::canMoveBackwardWithoutCapture(const Coords& start, const Coords& destination) const {
    if(hasCapturedThisTurn)
        return false;

    if(player1Turn) {
        if(board[start.x][start.y] == player1KingPiece
        && destination.y - start.y == 1
        && abs(destination.x - start.x) == 1)
            return true;
    } else {
        if(board[start.x][start.y] == player2KingPiece
        && start.y - destination.y == 1
        && abs(destination.x - start.x) == 1)
            return true;
    }

    return false;
}

void CheckersLogic::kinging(const Coords& pos) {
    if(pos.y == 0 && board[pos.x][pos.y] == player1Piece)
        board[pos.x][pos.y] = player1KingPiece;
    else if(pos.y == number_of_squares_in_board - 1 && board[pos.x][pos.y] == player2Piece)
        board[pos.x][pos.y] = player2KingPiece;
}

Coords CheckersLogic::movePos(const Coords& start, const Direction& direction, int step) {
    switch(direction) {
        case upLeftPos:
            return {start.x - step, start.y - step};
        case upPos:
            return {start.x, start.y - step};
        case upRightPos:
            return {start.x + step, start.y - step};
        case leftPos:
            return {start.x - step, start.y};
        case rightPos:
            return {start.x + step, start.y};
        case downLeftPos:
            return {start.x - step, start.y + step};
        case downPos:
            return {start.x, start.y + step};
        case downRightPos:
            return {start.x + step, start.y + step};
        default:
            return {0,0};
    }
}
