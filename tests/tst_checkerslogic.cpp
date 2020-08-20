#include "checkerslogic.h"

#include <QtTest>

enum Direction {upLeftPos, upPos, upRightPos, leftPos, rightPos, downLeftPos, downPos, downRightPos};

class CheckersLogicTests : public QObject {
    Q_OBJECT

private slots:
    void stateAfterResetTest();
    void playerCanOnlySelectOwnPiecesTest();
    void player1MoveWithoutCaptureTest();
    void player2MoveWithoutCaptureTest();
    void player1CaptureTest();
    void player2CaptureTest();
    void gameOverTest();

private:
    void emptyBoard(std::vector<std::vector<int>>& board);
    std::pair<int,int> move(const std::pair<int,int>& pos, const Direction& direction, int step = 1) const;
    void setBoardP1WinNoP2Pieces(std::vector<std::vector<int>>& board);
    void setBoardP1WinWithP2Pieces(std::vector<std::vector<int>>& board);
    void setBoardP2WinNoP1Pieces(std::vector<std::vector<int>>& board);
    void setBoardP2WinWithP1Pieces(std::vector<std::vector<int>>& board);
    std::vector<std::vector<int>> boardInitialState() const;

    const int numOfSquares = 8;
    CheckersLogic c{numOfSquares};
};

void CheckersLogicTests::stateAfterResetTest() {
    c.pieceSelected = true;
    c.player1Turn = false;
    c.hasCapturedThisTurn = true;

    c.resetBoard();

    QVERIFY(c.pieceSelected == false);
    QVERIFY(c.player1Turn == true);
    QVERIFY(c.hasCapturedThisTurn == false);

    QVERIFY(c.board == boardInitialState());
}

void CheckersLogicTests::playerCanOnlySelectOwnPiecesTest() {
    c.resetBoard();

    std::pair<int,int> arbitraryPlayerOnePiece {2, 5};
    std::pair<int,int> arbitraryPlayerTwoPiece {5, 2};

    // player 1
    QVERIFY(c.selectPiece(arbitraryPlayerOnePiece));
    QVERIFY(! c.selectPiece(arbitraryPlayerTwoPiece));

    // player 2
    c.player1Turn = false;
    QVERIFY(c.selectPiece(arbitraryPlayerTwoPiece));
    QVERIFY(! c.selectPiece(arbitraryPlayerOnePiece));
}

void CheckersLogicTests::player1MoveWithoutCaptureTest() {
    c.resetBoard();
    emptyBoard(c.board);


    std::pair<int,int> p1RegularPiece = {2, 5};
    c.board[p1RegularPiece.first][p1RegularPiece.second] = player1Piece;

    QVERIFY(c.isValid(p1RegularPiece, move(p1RegularPiece, upLeftPos)));
    QVERIFY(! c.isValid(p1RegularPiece, move(p1RegularPiece, upPos)));
    QVERIFY(c.isValid(p1RegularPiece, move(p1RegularPiece, upRightPos)));
    QVERIFY(! c.isValid(p1RegularPiece, move(p1RegularPiece, leftPos)));
    QVERIFY(! c.isValid(p1RegularPiece, move(p1RegularPiece, rightPos)));
    QVERIFY(! c.isValid(p1RegularPiece, move(p1RegularPiece, downLeftPos)));
    QVERIFY(! c.isValid(p1RegularPiece, move(p1RegularPiece, downPos)));
    QVERIFY(! c.isValid(p1RegularPiece, move(p1RegularPiece, downRightPos)));


    std::pair<int,int> p1KingPiece = {4, 5};
    c.board[p1KingPiece.first][p1KingPiece.second] = player1KingPiece;

    QVERIFY(c.isValid(p1KingPiece, move(p1KingPiece, upLeftPos)));
    QVERIFY(! c.isValid(p1KingPiece, move(p1KingPiece, upPos)));
    QVERIFY(c.isValid(p1KingPiece, move(p1KingPiece, upRightPos)));
    QVERIFY(! c.isValid(p1KingPiece, move(p1KingPiece, leftPos)));
    QVERIFY(! c.isValid(p1KingPiece, move(p1KingPiece, rightPos)));
    QVERIFY(c.isValid(p1KingPiece, move(p1KingPiece, downLeftPos)));
    QVERIFY(! c.isValid(p1KingPiece, move(p1KingPiece, downPos)));
    QVERIFY(c.isValid(p1KingPiece, move(p1KingPiece, downRightPos)));
}

void CheckersLogicTests::player2MoveWithoutCaptureTest() {
    c.resetBoard();
    emptyBoard(c.board);
    c.player1Turn = false;


    std::pair<int,int> p2RegularPiece = {3, 2};
    c.board[p2RegularPiece.first][p2RegularPiece.second] = player2Piece;

    QVERIFY(! c.isValid(p2RegularPiece, move(p2RegularPiece, upLeftPos)));
    QVERIFY(! c.isValid(p2RegularPiece, move(p2RegularPiece, upPos)));
    QVERIFY(! c.isValid(p2RegularPiece, move(p2RegularPiece, upRightPos)));
    QVERIFY(! c.isValid(p2RegularPiece, move(p2RegularPiece, leftPos)));
    QVERIFY(! c.isValid(p2RegularPiece, move(p2RegularPiece, rightPos)));
    QVERIFY(c.isValid(p2RegularPiece, move(p2RegularPiece, downLeftPos)));
    QVERIFY(! c.isValid(p2RegularPiece, move(p2RegularPiece, downPos)));
    QVERIFY(c.isValid(p2RegularPiece, move(p2RegularPiece, downRightPos)));


    std::pair<int,int> p2KingPiece = {5, 2};
    c.board[p2KingPiece.first][p2KingPiece.second] = player2KingPiece;

    QVERIFY(c.isValid(p2KingPiece, move(p2KingPiece, upLeftPos)));
    QVERIFY(! c.isValid(p2KingPiece, move(p2KingPiece, upPos)));
    QVERIFY(c.isValid(p2KingPiece, move(p2KingPiece, upRightPos)));
    QVERIFY(! c.isValid(p2KingPiece, move(p2KingPiece, leftPos)));
    QVERIFY(! c.isValid(p2KingPiece, move(p2KingPiece, rightPos)));
    QVERIFY(c.isValid(p2KingPiece, move(p2KingPiece, downLeftPos)));
    QVERIFY(! c.isValid(p2KingPiece, move(p2KingPiece, downPos)));
    QVERIFY(c.isValid(p2KingPiece, move(p2KingPiece, downRightPos)));
}

void CheckersLogicTests::player1CaptureTest() {
    c.resetBoard();
    emptyBoard(c.board);

    std::pair<int,int> p1RegularPiecePos = {2, 5};
    std::pair<int,int> p1KingPiecePos = {4, 5};
    c.board[p1RegularPiecePos.first][p1RegularPiecePos.second] = player1Piece;
    c.board[p1KingPiecePos.first][p1KingPiecePos.second] = player1KingPiece;

    c.board[move(p1RegularPiecePos, upLeftPos).first][move(p1RegularPiecePos, upLeftPos).second] = player1Piece;
    c.board[move(p1RegularPiecePos, downLeftPos).first][move(p1RegularPiecePos, downLeftPos).second] = player1Piece;

    c.board[move(p1RegularPiecePos, upRightPos).first][move(p1RegularPiecePos, upRightPos).second] = player2Piece;
    c.board[move(p1RegularPiecePos, downRightPos).first][move(p1RegularPiecePos, downRightPos).second] = player2Piece;

    c.board[move(p1KingPiecePos, upRightPos).first][move(p1KingPiecePos, upRightPos).second] = player1Piece;
    c.board[move(p1KingPiecePos, downRightPos).first][move(p1KingPiecePos, downRightPos).second] = player1Piece;

    QVERIFY(! c.isValid(p1RegularPiecePos, move(p1RegularPiecePos, upLeftPos, 2)));
    QVERIFY(! c.isValid(p1RegularPiecePos, move(p1RegularPiecePos, downLeftPos, 2)));
    QVERIFY(c.isValid(p1RegularPiecePos, move(p1RegularPiecePos, upRightPos, 2)));
    QVERIFY(! c.isValid(p1RegularPiecePos, move(p1RegularPiecePos, downRightPos, 2)));

    QVERIFY(c.isValid(p1KingPiecePos, move(p1KingPiecePos, upLeftPos, 2)));
    QVERIFY(c.isValid(p1KingPiecePos, move(p1KingPiecePos, downLeftPos, 2)));
    QVERIFY(! c.isValid(p1KingPiecePos, move(p1KingPiecePos, upRightPos, 2)));
    QVERIFY(! c.isValid(p1KingPiecePos, move(p1KingPiecePos, downRightPos, 2)));
}

void CheckersLogicTests::player2CaptureTest() {
    c.resetBoard();
    emptyBoard(c.board);
    c.player1Turn = false;

    std::pair<int,int> p2RegularPiecePos = {3, 2};
    std::pair<int,int> p2KingPiecePos = {5, 2};
    c.board[p2RegularPiecePos.first][p2RegularPiecePos.second] = player2Piece;
    c.board[p2KingPiecePos.first][p2KingPiecePos.second] = player2KingPiece;

    c.board[p2RegularPiecePos.first][p2RegularPiecePos.second] = player2Piece;
    c.board[p2KingPiecePos.first][p2KingPiecePos.second] = player2KingPiece;

    c.board[move(p2RegularPiecePos, upLeftPos).first][move(p2RegularPiecePos, upLeftPos).second] = player1Piece;
    c.board[move(p2RegularPiecePos, downLeftPos).first][move(p2RegularPiecePos, downLeftPos).second] = player1Piece;

    c.board[move(p2RegularPiecePos, upRightPos).first][move(p2RegularPiecePos, upRightPos).second] = player2Piece;
    c.board[move(p2RegularPiecePos, downRightPos).first][move(p2RegularPiecePos, downRightPos).second] = player2Piece;

    c.board[move(p2KingPiecePos, upRightPos).first][move(p2KingPiecePos, upRightPos).second] = player1Piece;
    c.board[move(p2KingPiecePos, downRightPos).first][move(p2KingPiecePos, downRightPos).second] = player1Piece;

    QVERIFY(! c.isValid(p2RegularPiecePos, move(p2RegularPiecePos, upLeftPos, 2)));
    QVERIFY(c.isValid(p2RegularPiecePos, move(p2RegularPiecePos, downLeftPos, 2)));
    QVERIFY(! c.isValid(p2RegularPiecePos, move(p2RegularPiecePos, upRightPos, 2)));
    QVERIFY(! c.isValid(p2RegularPiecePos, move(p2RegularPiecePos, downRightPos, 2)));

    QVERIFY(! c.isValid(p2KingPiecePos, move(p2KingPiecePos, upLeftPos, 2)));
    QVERIFY(! c.isValid(p2KingPiecePos, move(p2KingPiecePos, downLeftPos, 2)));
    QVERIFY(c.isValid(p2KingPiecePos, move(p2KingPiecePos, upRightPos, 2)));
    QVERIFY(c.isValid(p2KingPiecePos, move(p2KingPiecePos, downRightPos, 2)));
}

void CheckersLogicTests::gameOverTest() {
    c.resetBoard();

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
    for(auto& column : board)
        for(auto& square : column)
            square = empty;
}

std::pair<int,int> CheckersLogicTests::move(const std::pair<int,int>& pos, const Direction& direction, int step) const {
    switch(direction) {
        case upLeftPos:
            return {pos.first - step, pos.second - step};
        case upPos:
            return {pos.first, pos.second - step};
        case upRightPos:
            return {pos.first + step, pos.second - step};
        case leftPos:
            return {pos.first - step, pos.second};
        case rightPos:
            return {pos.first + step, pos.second};
        case downLeftPos:
            return {pos.first - step, pos.second + step};
        case downPos:
            return {pos.first, pos.second + step};
        case downRightPos:
            return {pos.first + step, pos.second + step};
        default:
            return {0,0};
    }
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

std::vector<std::vector<int>> CheckersLogicTests::boardInitialState() const {
    std::vector<std::vector<int>> board;
    board.resize(numOfSquares);
    for(auto& column : board)
        column.resize(numOfSquares);

    for(int i = 0; i < numOfSquares; ++i) {
        for(int j = 0; j < numOfSquares; ++j) {
            if(j < 3 && (i + j) % 2)
                board[i][j] = player2Piece;
            else if(j > 4 && (i + j) % 2)
                board[i][j] = player1Piece;
            else
                board[i][j] = empty;
        }
    }

    return board;
}

QTEST_APPLESS_MAIN(CheckersLogicTests)

#include "tst_checkerslogic.moc"
