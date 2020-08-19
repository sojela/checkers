#include "checkerslogic.h"

#include <QtTest>

class CheckersLogicTests : public QObject {
    Q_OBJECT

private slots:
    void gameTest();

private:
    const int numOfSquares = 8;
    void emptyBoard(std::vector<std::vector<int>>& board);
    std::pair<int,int> upLeft(std::pair<int,int> pos) const;
    std::pair<int,int> up(std::pair<int,int> pos) const;
    std::pair<int,int> upRight(std::pair<int,int> pos) const;
    std::pair<int,int> left(std::pair<int,int> pos) const;
    std::pair<int,int> right(std::pair<int,int> pos) const;
    std::pair<int,int> downLeft(std::pair<int,int> pos) const;
    std::pair<int,int> down(std::pair<int,int> pos) const;
    std::pair<int,int> downRight(std::pair<int,int> pos) const;
    std::pair<int,int> captureUpLeft(std::pair<int,int> pos) const;
    std::pair<int,int> captureUpRight(std::pair<int,int> pos) const;
    std::pair<int,int> captureDownLeft(std::pair<int,int> pos) const;
    std::pair<int,int> captureDownRight(std::pair<int,int> pos) const;
    void setBoardP1WinNoP2Pieces(std::vector<std::vector<int>>& board);
    void setBoardP1WinWithP2Pieces(std::vector<std::vector<int>>& board);
    void setBoardP2WinNoP1Pieces(std::vector<std::vector<int>>& board);
    void setBoardP2WinWithP1Pieces(std::vector<std::vector<int>>& board);
};

void CheckersLogicTests::gameTest() {
    CheckersLogic c(numOfSquares);


    // check game is in correct state after reset

    c.pieceSelected = true;
    c.player1Turn = false;
    c.hasCapturedThisTurn = true;

    c.resetBoard();

    QVERIFY(c.pieceSelected == false);
    QVERIFY(c.player1Turn == true);
    QVERIFY(c.hasCapturedThisTurn == false);


    // check player can only select their own pieces

    std::pair<int,int> playerOnePiece {2, 5};
    std::pair<int,int> playerTwoPiece {5, 2};

    // player 1
    QVERIFY(c.selectPiece(playerOnePiece));
    QVERIFY(! c.selectPiece(playerTwoPiece));

    // player 2
    c.player1Turn = false;
    QVERIFY(c.selectPiece(playerTwoPiece));
    QVERIFY(! c.selectPiece(playerOnePiece));


    // setup board for movement tests

    c.resetBoard();
    emptyBoard(c.board);

    std::pair<int,int> p1RegularPiecePos = {2, 5};
    std::pair<int,int> p1KingPiecePos = {4, 5};
    std::pair<int,int> p2RegularPiecePos = {3, 2};
    std::pair<int,int> p2KingPiecePos = {5, 2};

    c.board[p1RegularPiecePos.first][p1RegularPiecePos.second] = player1Piece;
    c.board[p1KingPiecePos.first][p1KingPiecePos.second] = player1KingPiece;
    c.board[p2RegularPiecePos.first][p2RegularPiecePos.second] = player2Piece;
    c.board[p2KingPiecePos.first][p2KingPiecePos.second] = player2KingPiece;

    // player 1 regular piece movement
    std::pair<int,int> upLeftPos = upLeft(p1RegularPiecePos);
    std::pair<int,int> upPos = up(p1RegularPiecePos);
    std::pair<int,int> upRightPos = upRight(p1RegularPiecePos);
    std::pair<int,int> leftPos = left(p1RegularPiecePos);
    std::pair<int,int> rightPos = right(p1RegularPiecePos);
    std::pair<int,int> downLeftPos = downLeft(p1RegularPiecePos);
    std::pair<int,int> downPos = down(p1RegularPiecePos);
    std::pair<int,int> downRightPos = downRight(p1RegularPiecePos);

    QVERIFY(c.isValid(p1RegularPiecePos, upLeftPos));
    QVERIFY(! c.isValid(p1RegularPiecePos, upPos));
    QVERIFY(c.isValid(p1RegularPiecePos, upRightPos));
    QVERIFY(! c.isValid(p1RegularPiecePos, leftPos));
    QVERIFY(! c.isValid(p1RegularPiecePos, rightPos));
    QVERIFY(! c.isValid(p1RegularPiecePos, downLeftPos));
    QVERIFY(! c.isValid(p1RegularPiecePos, downPos));
    QVERIFY(! c.isValid(p1RegularPiecePos, downRightPos));

    // player 1 king piece movement
    upLeftPos = upLeft(p1KingPiecePos);
    upPos = up(p1KingPiecePos);
    upRightPos = upRight(p1KingPiecePos);
    leftPos = left(p1KingPiecePos);
    rightPos = right(p1KingPiecePos);
    downLeftPos = downLeft(p1KingPiecePos);
    downPos = down(p1KingPiecePos);
    downRightPos = downRight(p1KingPiecePos);

    QVERIFY(c.isValid(p1KingPiecePos, upLeftPos));
    QVERIFY(! c.isValid(p1KingPiecePos, upPos));
    QVERIFY(c.isValid(p1KingPiecePos, upRightPos));
    QVERIFY(! c.isValid(p1KingPiecePos, leftPos));
    QVERIFY(! c.isValid(p1KingPiecePos, rightPos));
    QVERIFY(c.isValid(p1KingPiecePos, downLeftPos));
    QVERIFY(! c.isValid(p1KingPiecePos, downPos));
    QVERIFY(c.isValid(p1KingPiecePos, downRightPos));

    c.player1Turn = false;
    // player 2 regular piece movement
    upLeftPos = upLeft(p2RegularPiecePos);
    upPos = up(p2RegularPiecePos);
    upRightPos = upRight(p2RegularPiecePos);
    leftPos = left(p2RegularPiecePos);
    rightPos = right(p2RegularPiecePos);
    downLeftPos = downLeft(p2RegularPiecePos);
    downPos = down(p2RegularPiecePos);
    downRightPos = downRight(p2RegularPiecePos);

    QVERIFY(! c.isValid(p2RegularPiecePos, upLeftPos));
    QVERIFY(! c.isValid(p2RegularPiecePos, upPos));
    QVERIFY(! c.isValid(p2RegularPiecePos, upRightPos));
    QVERIFY(! c.isValid(p2RegularPiecePos, leftPos));
    QVERIFY(! c.isValid(p2RegularPiecePos, rightPos));
    QVERIFY(c.isValid(p2RegularPiecePos, downLeftPos));
    QVERIFY(! c.isValid(p2RegularPiecePos, downPos));
    QVERIFY(c.isValid(p2RegularPiecePos, downRightPos));

    // player 2 king piece movement
    upLeftPos = upLeft(p2KingPiecePos);
    upPos = up(p2KingPiecePos);
    upRightPos = upRight(p2KingPiecePos);
    leftPos = left(p2KingPiecePos);
    rightPos = right(p2KingPiecePos);
    downLeftPos = downLeft(p2KingPiecePos);
    downPos = down(p2KingPiecePos);
    downRightPos = downRight(p2KingPiecePos);

    QVERIFY(c.isValid(p2KingPiecePos, upLeftPos));
    QVERIFY(! c.isValid(p2KingPiecePos, upPos));
    QVERIFY(c.isValid(p2KingPiecePos, upRightPos));
    QVERIFY(! c.isValid(p2KingPiecePos, leftPos));
    QVERIFY(! c.isValid(p2KingPiecePos, rightPos));
    QVERIFY(c.isValid(p2KingPiecePos, downLeftPos));
    QVERIFY(! c.isValid(p2KingPiecePos, downPos));
    QVERIFY(c.isValid(p2KingPiecePos, downRightPos));


    // setup board for player 1 capture tests
    emptyBoard(c.board);
    c.player1Turn = true;

    c.board[p1RegularPiecePos.first][p1RegularPiecePos.second] = player1Piece;
    c.board[p1KingPiecePos.first][p1KingPiecePos.second] = player1KingPiece;

    c.board[upLeft(p1RegularPiecePos).first][upLeft(p1RegularPiecePos).second] = player1Piece;
    c.board[downLeft(p1RegularPiecePos).first][downLeft(p1RegularPiecePos).second] = player1Piece;

    c.board[upRight(p1RegularPiecePos).first][upRight(p1RegularPiecePos).second] = player2Piece;
    c.board[downRight(p1RegularPiecePos).first][downRight(p1RegularPiecePos).second] = player2Piece;

    c.board[upRight(p1KingPiecePos).first][upRight(p1KingPiecePos).second] = player1Piece;
    c.board[downRight(p1KingPiecePos).first][downRight(p1KingPiecePos).second] = player1Piece;

    // player 1 regular piece
    QVERIFY(! c.isValid(p1RegularPiecePos, captureUpLeft(p1RegularPiecePos)));
    QVERIFY(! c.isValid(p1RegularPiecePos, captureDownLeft(p1RegularPiecePos)));
    QVERIFY(c.isValid(p1RegularPiecePos, captureUpRight(p1RegularPiecePos)));
    QVERIFY(! c.isValid(p1RegularPiecePos, captureDownRight(p1RegularPiecePos)));
    // player 1 king piece
    QVERIFY(c.isValid(p1KingPiecePos, captureUpLeft(p1KingPiecePos)));
    QVERIFY(c.isValid(p1KingPiecePos, captureDownLeft(p1KingPiecePos)));
    QVERIFY(! c.isValid(p1KingPiecePos, captureUpRight(p1KingPiecePos)));
    QVERIFY(! c.isValid(p1KingPiecePos, captureDownRight(p1KingPiecePos)));

    // setup board for player 2 capture tests
    emptyBoard(c.board);
    c.player1Turn = false;

    c.board[p2RegularPiecePos.first][p2RegularPiecePos.second] = player2Piece;
    c.board[p2KingPiecePos.first][p2KingPiecePos.second] = player2KingPiece;

    c.board[upLeft(p2RegularPiecePos).first][upLeft(p2RegularPiecePos).second] = player1Piece;
    c.board[downLeft(p2RegularPiecePos).first][downLeft(p2RegularPiecePos).second] = player1Piece;

    c.board[upRight(p2RegularPiecePos).first][upRight(p2RegularPiecePos).second] = player2Piece;
    c.board[downRight(p2RegularPiecePos).first][downRight(p2RegularPiecePos).second] = player2Piece;

    c.board[upRight(p2KingPiecePos).first][upRight(p2KingPiecePos).second] = player1Piece;
    c.board[downRight(p2KingPiecePos).first][downRight(p2KingPiecePos).second] = player1Piece;

    // player 2 regular piece
    QVERIFY(! c.isValid(p2RegularPiecePos, captureUpLeft(p2RegularPiecePos)));
    QVERIFY(c.isValid(p2RegularPiecePos, captureDownLeft(p2RegularPiecePos)));
    QVERIFY(! c.isValid(p2RegularPiecePos, captureUpRight(p2RegularPiecePos)));
    QVERIFY(! c.isValid(p2RegularPiecePos, captureDownRight(p2RegularPiecePos)));
    // player 2 king piece
    QVERIFY(! c.isValid(p2KingPiecePos, captureUpLeft(p2KingPiecePos)));
    QVERIFY(! c.isValid(p2KingPiecePos, captureDownLeft(p2KingPiecePos)));
    QVERIFY(c.isValid(p2KingPiecePos, captureUpRight(p2KingPiecePos)));
    QVERIFY(c.isValid(p2KingPiecePos, captureDownRight(p2KingPiecePos)));


    // game over states

    const int player1 = 1;
    const int player2 = 2;

    setBoardP1WinNoP2Pieces(c.board);
    QVERIFY(c.gameOver() == player1);

    c.player1Turn = false;
    setBoardP1WinWithP2Pieces(c.board);
    QVERIFY(c.gameOver() == player1);

    setBoardP2WinNoP1Pieces(c.board);
    QVERIFY(c.gameOver() == player2);

    c.player1Turn = true;
    setBoardP2WinWithP1Pieces(c.board);
    QVERIFY(c.gameOver() == player2);
}

void CheckersLogicTests::emptyBoard(std::vector<std::vector<int>>& board) {
    for(auto& column : board) {
        for(auto& square : column) {
            square = empty;
        }
    }
}

std::pair<int,int> CheckersLogicTests::upLeft(std::pair<int,int> pos) const {
    return {pos.first - 1, pos.second - 1};
}

std::pair<int,int> CheckersLogicTests::up(std::pair<int,int> pos) const {
    return {pos.first, pos.second - 1};
}

std::pair<int,int> CheckersLogicTests::upRight(std::pair<int,int> pos) const {
    return {pos.first + 1, pos.second - 1};
}

std::pair<int,int> CheckersLogicTests::left(std::pair<int,int> pos) const {
    return {pos.first - 1, pos.second};
}

std::pair<int,int> CheckersLogicTests::right(std::pair<int,int> pos) const {
    return {pos.first + 1, pos.second};
}

std::pair<int,int> CheckersLogicTests::downLeft(std::pair<int,int> pos) const {
    return {pos.first - 1, pos.second + 1};
}

std::pair<int,int> CheckersLogicTests::down(std::pair<int,int> pos) const {
    return {pos.first, pos.second + 1};
}

std::pair<int,int> CheckersLogicTests::downRight(std::pair<int,int> pos) const {
    return {pos.first + 1, pos.second + 1};
}

std::pair<int,int> CheckersLogicTests::captureUpLeft(std::pair<int,int> pos) const {
    return upLeft(upLeft(pos));
}

std::pair<int,int> CheckersLogicTests::captureUpRight(std::pair<int,int> pos) const {
    return upRight(upRight(pos));
}

std::pair<int,int> CheckersLogicTests::captureDownLeft(std::pair<int,int> pos) const {
    return downLeft(downLeft(pos));
}

std::pair<int,int> CheckersLogicTests::captureDownRight(std::pair<int,int> pos) const {
    return downRight(downRight(pos));
}

void CheckersLogicTests::setBoardP1WinNoP2Pieces(std::vector<std::vector<int>>& board) {
    emptyBoard(board);

    board[2][5] = player1Piece;
}

void CheckersLogicTests::setBoardP1WinWithP2Pieces(std::vector<std::vector<int>>& board) {
    emptyBoard(board);

    board[0][3] = player2Piece;
    board[1][4] = player1Piece;
    board[2][5] = player1Piece;
}

void CheckersLogicTests::setBoardP2WinNoP1Pieces(std::vector<std::vector<int>>& board) {
    emptyBoard(board);

    board[2][5] = player2Piece;
}

void CheckersLogicTests::setBoardP2WinWithP1Pieces(std::vector<std::vector<int>>& board) {
    emptyBoard(board);

    board[7][6] = player1Piece;
    board[6][5] = player2Piece;
    board[5][4] = player2Piece;
}

QTEST_APPLESS_MAIN(CheckersLogicTests)

#include "tst_checkerslogictests.moc"
