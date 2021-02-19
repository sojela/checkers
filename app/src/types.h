#ifndef TYPES_H
#define TYPES_H

#include <utility>

struct Coords {
    int x, y;

    Coords() {}

    Coords(std::pair<int, int> pair) {
        x = pair.first;
        y = pair.second;
    }

    operator std::pair<int, int>() {
        return {x, y};
    }

    Coords(int nx, int ny) {
        x = nx;
        y = ny;
    }
};

enum PieceType {empty, player1Piece, player2Piece, player1KingPiece, player2KingPiece};
enum GameState {current, player1Wins, player2Wins};
enum Direction {upLeftPos, upRightPos, downLeftPos, downRightPos};
enum TypeOfGame {localPvP, PvAI};
enum Difficulty {veryEasy, easy};

#endif // TYPES_H
