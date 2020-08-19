#include "checkersai.h"
#include "checkers.h"

#include <vector>
#include <ctime>
#include <cstdlib>

extern Checkers* MainWindow;

std::pair<std::pair<int, int>, std::pair<int, int>> CheckersAI::calculateMoveVeryEasy(CheckersLogic currentGameState) {
    std::vector<std::pair<int, int>> moveablePieces;
    std::pair<int, int> randomPiece;

    srand(time(NULL));

    if(currentGameState.hasCapturedThisTurn)
        randomPiece = currentGameState.selectedPiece;
    else {
        for(int i = 0; i < MainWindow->number_of_squares_in_board; ++i) {
            for(int j = 0; j < MainWindow->number_of_squares_in_board; ++j) {
                if(currentGameState.isMoveable({i, j})) {
                    moveablePieces.push_back({i, j});
                }
            }
        }

        int randomNumber = rand() % moveablePieces.size();

        randomPiece = moveablePieces[randomNumber];
    }

    std::vector<std::pair<int, int>> availableMovesForChosenPiece;

    for(int i = 0; i < MainWindow->number_of_squares_in_board; ++i) {
        for(int j = 0; j < MainWindow->number_of_squares_in_board; ++j) {
            if(currentGameState.isValid(randomPiece, {i, j})) {
                availableMovesForChosenPiece.push_back({i, j});
            }
        }
    }

    int randomNumber = rand() % availableMovesForChosenPiece.size();

    std::pair<int, int> randomMove = availableMovesForChosenPiece[randomNumber];

    return {randomPiece, randomMove};
}
