#include "checkersai.h"
#include "checkers.h"

#include <ctime>
#include <cstdlib>

extern Checkers* MainWindow;

// chooses move randomly
Move CheckersAI::calculateMoveVeryEasy(const CheckersLogic& currentGameState) {
    // choose a piece to move
    Coords pieceToMove;
    std::vector<Coords> moveablePieces;

    if(currentGameState.hasCapturedThisTurn)
        pieceToMove = currentGameState.selectedPiece;
    else {
        for(int i = 0; i < MainWindow->number_of_squares_in_board; ++i)
            for(int j = 0; j < MainWindow->number_of_squares_in_board; ++j)
                if(currentGameState.isMoveable({i, j}))
                    moveablePieces.push_back({i, j});

        pieceToMove = moveablePieces[randomNumber(moveablePieces.size())];
    }


    // choose a move for the selected piece
    auto possibleMoves = calculateAllMovesForPiece(pieceToMove);

    std::vector<Coords> availableMovesForChosenPiece;

    for(auto& destination : possibleMoves) {
        bool outOfBounds = destination.x < 0
                        || destination.y < 0
                        || destination.x >= currentGameState.number_of_squares_in_board
                        || destination.y >= currentGameState.number_of_squares_in_board;

        if(outOfBounds) continue;

        if(currentGameState.isValidMove(pieceToMove, destination))
            availableMovesForChosenPiece.push_back(destination);
    }

    Coords move = availableMovesForChosenPiece[randomNumber(availableMovesForChosenPiece.size())];

    return {pieceToMove, move};
}

std::vector<Coords> CheckersAI::calculateAllMovesForPiece(const Coords& piece) {
    std::vector<Coords> result;
    result.reserve(8);

    result.push_back(CheckersLogic::movePos(piece, upLeftPos));
    result.push_back(CheckersLogic::movePos(piece, upRightPos));
    result.push_back(CheckersLogic::movePos(piece, downLeftPos));
    result.push_back(CheckersLogic::movePos(piece, downRightPos));
    result.push_back(CheckersLogic::movePos(piece, upLeftPos, 2));
    result.push_back(CheckersLogic::movePos(piece, upRightPos, 2));
    result.push_back(CheckersLogic::movePos(piece, downLeftPos, 2));
    result.push_back(CheckersLogic::movePos(piece, downRightPos, 2));

    return result;
}

int CheckersAI::randomNumber(unsigned int max) {
    srand(time(NULL));
    return rand() % max;
}
