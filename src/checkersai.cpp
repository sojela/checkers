#include "checkersai.h"
#include "checkers.h"

#include <vector>
#include <ctime>
#include <cstdlib>

extern Checkers* w;

std::pair<std::pair<int, int>, std::pair<int, int> > CheckersAI::calculateMove(const QVector<QVector<std::pair<std::shared_ptr<CheckersSquare>, std::shared_ptr<CheckersPiece>>>>& board) {
    std::vector<std::pair<int, int>> moveablePieces;
    std::pair<int, int> randomPiece;

    srand(time(NULL));

    if(w->hasCapturedThisTurn) {
        randomPiece = w->selectedPiece;
    } else {
        for(int i = 0; i < w->board_length; ++i) {
            for(int j = 0; j < w->board_length; ++j) {
                if(board[i][j].second && w->isMoveable(board[i][j].second)) {
                    moveablePieces.push_back({i, j});
                }
            }
        }

        int randomNumber = rand() % moveablePieces.size();

        randomPiece = moveablePieces[randomNumber];
    }

    std::vector<std::pair<int, int>> availableMovesForChosenPiece;

    for(int i = 0; i < w->board_length; ++i) {
        for(int j = 0; j < w->board_length; ++j) {
            if(w->isValid(randomPiece, {i, j})) {
                availableMovesForChosenPiece.push_back({i, j});
            }
        }
    }

    int randomNumber = rand() % availableMovesForChosenPiece.size();

    std::pair<int, int> randomMove = availableMovesForChosenPiece[randomNumber];

    return {randomPiece, randomMove};
}
