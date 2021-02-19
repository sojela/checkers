#include "checkerslogic.h"

#include <cstdlib>

struct CheckersLogic::Impl {
    struct upLeftPos_tag{};
    struct upRightPos_tag{};
    struct downLeftPos_tag{};
    struct downRightPos_tag{};

    Impl();
    void removeCapturedPiece(const Coords& start, const Coords& end);
    bool canMoveWithoutCapture(const Coords& pos) const;
    bool canCaptureForwards(const Coords& start, const Coords& destination) const;
    bool canCaptureBackwards(const Coords& start, const Coords& destination) const;
    bool canMoveForwardsWithoutCapture(const Coords& start, const Coords& destination) const;
    bool canMoveBackwardWithoutCapture(const Coords& start, const Coords& destination) const;
    bool canMove(const Coords& pos, unsigned int step) const;
    void endTurn();
    void kinging(const Coords& pos);
    bool isValidMove(const Coords& start, const Coords& destination) const;
    bool isCurrentPlayersPiece(const Coords& pos) const;
    bool isEnemyPiece(const Coords& pos) const;
    GameState getGameState() const;
    bool isMoveable(const Coords& piecePos) const;
    bool canCapture(const Coords& pos) const;
    Coords movePos(const Coords& start, upLeftPos_tag, int step) const;
    Coords movePos(const Coords& start, upRightPos_tag, int step) const;
    Coords movePos(const Coords& start, downLeftPos_tag, int step) const;
    Coords movePos(const Coords& start, downRightPos_tag, int step) const;

    Coords captured;
    std::array<std::array<int, number_of_squares_in_board>, number_of_squares_in_board> board;
    Coords selectedPiece;
    bool pieceSelected;
    bool player1Turn;
    bool hasCapturedThisTurn;
    GameState currentGameState;
};

CheckersLogic::CheckersLogic()
    : pimpl(new Impl) {}

CheckersLogic::CheckersLogic(const CheckersLogic& state) : CheckersLogic::CheckersLogic() {
    pimpl->board = state.getBoard();
    pimpl->pieceSelected = state.pimpl->pieceSelected;
    pimpl->player1Turn = state.pimpl->player1Turn;
    pimpl->hasCapturedThisTurn = state.pimpl->hasCapturedThisTurn;
    pimpl->currentGameState = state.pimpl->currentGameState;
    pimpl->selectedPiece = state.pimpl->selectedPiece;
    pimpl->captured = state.pimpl->captured;
}

CheckersLogic::~CheckersLogic() = default;

CheckersLogic::Impl::Impl()
    : pieceSelected(false),
      player1Turn(true),
      hasCapturedThisTurn(false),
      currentGameState(current)
{}

void CheckersLogic::Impl::removeCapturedPiece(const Coords& start, const Coords& end) {
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
                    pimpl->board[i][j] = player2Piece;
                else if(bottomOfBoard)
                    pimpl->board[i][j] = player1Piece;
                else
                    pimpl->board[i][j] = empty;
            } else
                pimpl->board[i][j] = empty;
        }
    }

    pimpl->pieceSelected = false;
    pimpl->player1Turn = true;
    pimpl->hasCapturedThisTurn = false;
    pimpl->currentGameState = current;
}

bool CheckersLogic::selectPiece(const Coords& pos) {
    if(pimpl->isMoveable(pos)) {
        pimpl->selectedPiece = pos;
        pimpl->pieceSelected = true;
        return true;
    } else
        return false;
}

void CheckersLogic::Impl::endTurn() {
    hasCapturedThisTurn = false;
    pieceSelected = false;

    if(player1Turn)
        player1Turn = false;
    else
        player1Turn = true;

    currentGameState = getGameState();
}

// assume valid coordinates
bool CheckersLogic::Impl::isValidMove(const Coords& start, const Coords& destination) const {
    if(board[start.x][start.y] == empty
            || board[destination.x][destination.y] != empty
            || !isCurrentPlayersPiece(start))
        return false;

    if(canMoveForwardsWithoutCapture(start, destination)
            || canCaptureForwards(start, destination)
            || canMoveBackwardWithoutCapture(start, destination)
            || canCaptureBackwards(start, destination))
        return true;

    return false;
}

bool CheckersLogic::Impl::canCapture(const Coords& pos) const {
    return canMove(pos, 2);
}

Coords CheckersLogic::Impl::movePos(const Coords& start, CheckersLogic::Impl::upLeftPos_tag, int step) const {
    return {start.x - step, start.y - step};
}

Coords CheckersLogic::Impl::movePos(const Coords& start, CheckersLogic::Impl::upRightPos_tag, int step) const {
    return {start.x + step, start.y - step};
}

Coords CheckersLogic::Impl::movePos(const Coords& start, CheckersLogic::Impl::downLeftPos_tag, int step) const {
    return {start.x - step, start.y + step};
}

Coords CheckersLogic::Impl::movePos(const Coords& start, CheckersLogic::Impl::downRightPos_tag, int step) const {
    return {start.x + step, start.y + step};
}

bool CheckersLogic::Impl::canMoveWithoutCapture(const Coords& pos) const {
    return canMove(pos, 1);
}

bool CheckersLogic::Impl::canMove(const Coords& pos, unsigned int step) const {
    if(board[pos.x][pos.y] != player2Piece) {
        auto upLeft = movePos(pos, upLeftPos_tag(), step);
        if(upLeft.x >= 0 && upLeft.y >= 0)
            if(board[upLeft.x][upLeft.y] == empty) {
                if(step == 1)
                    return true;
                else if(isEnemyPiece({pos.x - 1, pos.y - 1}))
                        return true;
            }

        auto upRight = movePos(pos, upRightPos_tag(), step);
        if(upRight.x < number_of_squares_in_board && upRight.y >= 0)
            if(board[upRight.x][upRight.y] == empty) {
                if(step == 1)
                    return true;
                else if(isEnemyPiece({pos.x + 1, pos.y - 1}))
                        return true;
            }
    }

    if(board[pos.x][pos.y] != player1Piece) {
        auto downLeft = movePos(pos, downLeftPos_tag(), step);
        if(downLeft.x >= 0 && downLeft.y < number_of_squares_in_board)
            if(board[downLeft.x][downLeft.y] == empty) {
                if(step == 1)
                    return true;
                else if(isEnemyPiece({pos.x - 1, pos.y + 1}))
                        return true;
            }

        auto downRight = movePos(pos, downRightPos_tag(), step);
        if(downRight.x < number_of_squares_in_board && downRight.y < number_of_squares_in_board)
            if(board[downRight.x][downRight.y] == empty) {
                if(step == 1)
                    return true;
                else if(isEnemyPiece({pos.x + 1, pos.y + 1}))
                        return true;
            }
    }

    return false;
}

bool CheckersLogic::Impl::isMoveable(const Coords& piecePos) const {
    if(board[piecePos.x][piecePos.y] == empty
            || !isCurrentPlayersPiece({piecePos.x, piecePos.y}))
        return false;

    if(canMoveWithoutCapture(piecePos)
            || canCapture(piecePos))
        return true;

    return false;
}

bool CheckersLogic::Impl::canCaptureForwards(const Coords& start, const Coords& destination) const {
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

bool CheckersLogic::Impl::canCaptureBackwards(const Coords& start, const Coords& destination) const {
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
bool CheckersLogic::Impl::isCurrentPlayersPiece(const Coords& pos) const {
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

bool CheckersLogic::Impl::isEnemyPiece(const Coords& pos) const {
    auto piece = board[pos.x][pos.y];

    if(player1Turn) {
        if(piece == player2Piece || piece == player2KingPiece)
            return true;
    } else {
        if(piece == player1Piece || piece == player1KingPiece)
            return true;
    }

    return false;
}

GameState CheckersLogic::getGameState() const {
    return pimpl->currentGameState;
}

void CheckersLogic::movePiece(const Coords& destination) {
    if(!pimpl->pieceSelected || pimpl->currentGameState != current) return;

    if(pimpl->isValidMove(pimpl->selectedPiece, destination)) {
        pimpl->board[destination.x][destination.y] = pimpl->board[pimpl->selectedPiece.x][pimpl->selectedPiece.y];
        pimpl->board[pimpl->selectedPiece.x][pimpl->selectedPiece.y] = empty;

        if(abs(destination.x - pimpl->selectedPiece.x) == 2) {
            pimpl->removeCapturedPiece(pimpl->selectedPiece, destination);
            if(pimpl->canCapture(destination)) {
                pimpl->hasCapturedThisTurn = true;
                pimpl->selectedPiece = destination;
                return;
            }
        }

        pimpl->kinging(destination);
        pimpl->endTurn();
    }
}

GameState CheckersLogic::Impl::getGameState() const {
    for(int i = 0; i < number_of_squares_in_board; ++i)
        for(int j = 0; j < number_of_squares_in_board; ++j)
            if(isMoveable({i, j}))
                return current;

    if(player1Turn)
        return player2Wins;
    else
        return player1Wins;
}

bool CheckersLogic::Impl::canMoveForwardsWithoutCapture(const Coords& start, const Coords& destination) const {
    if(player1Turn) {
        if(start.y - destination.y == 1 && abs(destination.x - start.x) == 1)
            return true;
    } else if(destination.y - start.y == 1 && abs(destination.x - start.x) == 1)
        return true;

    return false;
}

bool CheckersLogic::Impl::canMoveBackwardWithoutCapture(const Coords& start, const Coords& destination) const {
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

void CheckersLogic::Impl::kinging(const Coords& pos) {
    if(pos.y == 0 && board[pos.x][pos.y] == player1Piece)
        board[pos.x][pos.y] = player1KingPiece;
    else if(pos.y == number_of_squares_in_board - 1 && board[pos.x][pos.y] == player2Piece)
        board[pos.x][pos.y] = player2KingPiece;
}

Coords CheckersLogic::movePos(const Coords& start, const Direction& direction, int step) {
    switch(direction) {
        case upLeftPos:
            return {start.x - step, start.y - step};
        case upRightPos:
            return {start.x + step, start.y - step};
        case downLeftPos:
            return {start.x - step, start.y + step};
        case downRightPos:
            return {start.x + step, start.y + step};
    }
}

const std::array<std::array<int, CheckersLogic::number_of_squares_in_board>, CheckersLogic::number_of_squares_in_board>& CheckersLogic::getBoard() const {
    return pimpl->board;
}

const Coords& CheckersLogic::getSelectedPiece() const {
    return pimpl->selectedPiece;
}

bool CheckersLogic::isPieceSelected() const {
    return pimpl->pieceSelected;
}

bool CheckersLogic::hasCapturedThisTurn() const {
    return pimpl->hasCapturedThisTurn;
}

bool CheckersLogic::isValidMove(const Coords& start, const Coords& destination) const {
    return pimpl->isValidMove(start, destination);
}

bool CheckersLogic::isPlayer1Turn() const {
    return pimpl->player1Turn;
}

bool CheckersLogic::isMoveable(const Coords& piecePos) const {
    return pimpl->isMoveable(piecePos);
}

bool CheckersLogic::isCurrentPlayersPiece(const Coords& pos) const {
    return pimpl->isCurrentPlayersPiece(pos);
}
