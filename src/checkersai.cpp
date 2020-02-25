#include "checkersai.h"
#include "checkers.h"

#include <vector>
#include <ctime>
#include <cstdlib>

extern Checkers* MainWindow;

std::pair<std::pair<int, int>, std::pair<int, int> > CheckersAI::calculateMoveVeryEasy(const QVector<QVector<std::pair<std::shared_ptr<CheckersSquare>, std::shared_ptr<CheckersPiece>>>>& board) {
    std::vector<std::pair<int, int>> moveablePieces;
    std::pair<int, int> randomPiece;

    srand(time(NULL));

    if(MainWindow->hasCapturedThisTurn) {
        randomPiece = MainWindow->selectedPiece;
    } else {
        for(int i = 0; i < MainWindow->number_of_squares_in_board; ++i) {
            for(int j = 0; j < MainWindow->number_of_squares_in_board; ++j) {
                if(board[i][j].second && MainWindow->isMoveable(board[i][j].second)) {
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
            if(MainWindow->isValid(randomPiece, {i, j})) {
                availableMovesForChosenPiece.push_back({i, j});
            }
        }
    }

    int randomNumber = rand() % availableMovesForChosenPiece.size();

    std::pair<int, int> randomMove = availableMovesForChosenPiece[randomNumber];

    return {randomPiece, randomMove};
}
