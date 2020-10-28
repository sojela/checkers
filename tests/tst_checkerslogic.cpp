#include "checkerslogic.h"

#include <QtTest>

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
    void setBoardP1WinNoP2Pieces(std::vector<std::vector<int>>& board);
    void setBoardP1WinWithP2Pieces(std::vector<std::vector<int>>& board);
    void setBoardP2WinNoP1Pieces(std::vector<std::vector<int>>& board);
    void setBoardP2WinWithP1Pieces(std::vector<std::vector<int>>& board);
    std::vector<std::vector<int>> boardInitialState() const;

    const int numOfSquares = 8;
    CheckersLogic c;
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

    Coords arbitraryPlayerOnePiece {2, 5};
    Coords arbitraryPlayerTwoPiece {5, 2};

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


    Coords p1RegularPiece = {2, 5};
    c.board[p1RegularPiece.x][p1RegularPiece.y] = player1Piece;

    QVERIFY(c.isValidMove(p1RegularPiece, c.movePos(p1RegularPiece, upLeftPos)));
    QVERIFY(! c.isValidMove(p1RegularPiece, c.movePos(p1RegularPiece, upPos)));
    QVERIFY(c.isValidMove(p1RegularPiece, c.movePos(p1RegularPiece, upRightPos)));
    QVERIFY(! c.isValidMove(p1RegularPiece, c.movePos(p1RegularPiece, leftPos)));
    QVERIFY(! c.isValidMove(p1RegularPiece, c.movePos(p1RegularPiece, rightPos)));
    QVERIFY(! c.isValidMove(p1RegularPiece, c.movePos(p1RegularPiece, downLeftPos)));
    QVERIFY(! c.isValidMove(p1RegularPiece, c.movePos(p1RegularPiece, downPos)));
    QVERIFY(! c.isValidMove(p1RegularPiece, c.movePos(p1RegularPiece, downRightPos)));


    Coords p1KingPiece = {4, 5};
    c.board[p1KingPiece.x][p1KingPiece.y] = player1KingPiece;

    QVERIFY(c.isValidMove(p1KingPiece, c.movePos(p1KingPiece, upLeftPos)));
    QVERIFY(! c.isValidMove(p1KingPiece, c.movePos(p1KingPiece, upPos)));
    QVERIFY(c.isValidMove(p1KingPiece, c.movePos(p1KingPiece, upRightPos)));
    QVERIFY(! c.isValidMove(p1KingPiece, c.movePos(p1KingPiece, leftPos)));
    QVERIFY(! c.isValidMove(p1KingPiece, c.movePos(p1KingPiece, rightPos)));
    QVERIFY(c.isValidMove(p1KingPiece, c.movePos(p1KingPiece, downLeftPos)));
    QVERIFY(! c.isValidMove(p1KingPiece, c.movePos(p1KingPiece, downPos)));
    QVERIFY(c.isValidMove(p1KingPiece, c.movePos(p1KingPiece, downRightPos)));
}

void CheckersLogicTests::player2MoveWithoutCaptureTest() {
    c.resetBoard();
    emptyBoard(c.board);
    c.player1Turn = false;


    Coords p2RegularPiece = {3, 2};
    c.board[p2RegularPiece.x][p2RegularPiece.y] = player2Piece;

    QVERIFY(! c.isValidMove(p2RegularPiece, c.movePos(p2RegularPiece, upLeftPos)));
    QVERIFY(! c.isValidMove(p2RegularPiece, c.movePos(p2RegularPiece, upPos)));
    QVERIFY(! c.isValidMove(p2RegularPiece, c.movePos(p2RegularPiece, upRightPos)));
    QVERIFY(! c.isValidMove(p2RegularPiece, c.movePos(p2RegularPiece, leftPos)));
    QVERIFY(! c.isValidMove(p2RegularPiece, c.movePos(p2RegularPiece, rightPos)));
    QVERIFY(c.isValidMove(p2RegularPiece, c.movePos(p2RegularPiece, downLeftPos)));
    QVERIFY(! c.isValidMove(p2RegularPiece, c.movePos(p2RegularPiece, downPos)));
    QVERIFY(c.isValidMove(p2RegularPiece, c.movePos(p2RegularPiece, downRightPos)));


    Coords p2KingPiece = {5, 2};
    c.board[p2KingPiece.x][p2KingPiece.y] = player2KingPiece;

    QVERIFY(c.isValidMove(p2KingPiece, c.movePos(p2KingPiece, upLeftPos)));
    QVERIFY(! c.isValidMove(p2KingPiece, c.movePos(p2KingPiece, upPos)));
    QVERIFY(c.isValidMove(p2KingPiece, c.movePos(p2KingPiece, upRightPos)));
    QVERIFY(! c.isValidMove(p2KingPiece, c.movePos(p2KingPiece, leftPos)));
    QVERIFY(! c.isValidMove(p2KingPiece, c.movePos(p2KingPiece, rightPos)));
    QVERIFY(c.isValidMove(p2KingPiece, c.movePos(p2KingPiece, downLeftPos)));
    QVERIFY(! c.isValidMove(p2KingPiece, c.movePos(p2KingPiece, downPos)));
    QVERIFY(c.isValidMove(p2KingPiece, c.movePos(p2KingPiece, downRightPos)));
}

void CheckersLogicTests::player1CaptureTest() {
    c.resetBoard();
    emptyBoard(c.board);

    Coords p1RegularPiecePos = {2, 5};
    Coords p1KingPiecePos = {4, 5};
    c.board[p1RegularPiecePos.x][p1RegularPiecePos.y] = player1Piece;
    c.board[p1KingPiecePos.x][p1KingPiecePos.y] = player1KingPiece;

    c.board[c.movePos(p1RegularPiecePos, upLeftPos).x][c.movePos(p1RegularPiecePos, upLeftPos).y] = player1Piece;
    c.board[c.movePos(p1RegularPiecePos, downLeftPos).x][c.movePos(p1RegularPiecePos, downLeftPos).y] = player1Piece;

    c.board[c.movePos(p1RegularPiecePos, upRightPos).x][c.movePos(p1RegularPiecePos, upRightPos).y] = player2Piece;
    c.board[c.movePos(p1RegularPiecePos, downRightPos).x][c.movePos(p1RegularPiecePos, downRightPos).y] = player2Piece;

    c.board[c.movePos(p1KingPiecePos, upRightPos).x][c.movePos(p1KingPiecePos, upRightPos).y] = player1Piece;
    c.board[c.movePos(p1KingPiecePos, downRightPos).x][c.movePos(p1KingPiecePos, downRightPos).y] = player1Piece;

    QVERIFY(! c.isValidMove(p1RegularPiecePos, c.movePos(p1RegularPiecePos, upLeftPos, 2)));
    QVERIFY(! c.isValidMove(p1RegularPiecePos, c.movePos(p1RegularPiecePos, downLeftPos, 2)));
    QVERIFY(c.isValidMove(p1RegularPiecePos, c.movePos(p1RegularPiecePos, upRightPos, 2)));
    QVERIFY(! c.isValidMove(p1RegularPiecePos, c.movePos(p1RegularPiecePos, downRightPos, 2)));

    QVERIFY(c.isValidMove(p1KingPiecePos, c.movePos(p1KingPiecePos, upLeftPos, 2)));
    QVERIFY(c.isValidMove(p1KingPiecePos, c.movePos(p1KingPiecePos, downLeftPos, 2)));
    QVERIFY(! c.isValidMove(p1KingPiecePos, c.movePos(p1KingPiecePos, upRightPos, 2)));
    QVERIFY(! c.isValidMove(p1KingPiecePos, c.movePos(p1KingPiecePos, downRightPos, 2)));
}

void CheckersLogicTests::player2CaptureTest() {
    c.resetBoard();
    emptyBoard(c.board);
    c.player1Turn = false;

    Coords p2RegularPiecePos = {3, 2};
    Coords p2KingPiecePos = {5, 2};
    c.board[p2RegularPiecePos.x][p2RegularPiecePos.y] = player2Piece;
    c.board[p2KingPiecePos.x][p2KingPiecePos.y] = player2KingPiece;

    c.board[p2RegularPiecePos.x][p2RegularPiecePos.y] = player2Piece;
    c.board[p2KingPiecePos.x][p2KingPiecePos.y] = player2KingPiece;

    c.board[c.movePos(p2RegularPiecePos, upLeftPos).x][c.movePos(p2RegularPiecePos, upLeftPos).y] = player1Piece;
    c.board[c.movePos(p2RegularPiecePos, downLeftPos).x][c.movePos(p2RegularPiecePos, downLeftPos).y] = player1Piece;

    c.board[c.movePos(p2RegularPiecePos, upRightPos).x][c.movePos(p2RegularPiecePos, upRightPos).y] = player2Piece;
    c.board[c.movePos(p2RegularPiecePos, downRightPos).x][c.movePos(p2RegularPiecePos, downRightPos).y] = player2Piece;

    c.board[c.movePos(p2KingPiecePos, upRightPos).x][c.movePos(p2KingPiecePos, upRightPos).y] = player1Piece;
    c.board[c.movePos(p2KingPiecePos, downRightPos).x][c.movePos(p2KingPiecePos, downRightPos).y] = player1Piece;

    QVERIFY(! c.isValidMove(p2RegularPiecePos, c.movePos(p2RegularPiecePos, upLeftPos, 2)));
    QVERIFY(c.isValidMove(p2RegularPiecePos, c.movePos(p2RegularPiecePos, downLeftPos, 2)));
    QVERIFY(! c.isValidMove(p2RegularPiecePos, c.movePos(p2RegularPiecePos, upRightPos, 2)));
    QVERIFY(! c.isValidMove(p2RegularPiecePos, c.movePos(p2RegularPiecePos, downRightPos, 2)));

    QVERIFY(! c.isValidMove(p2KingPiecePos, c.movePos(p2KingPiecePos, upLeftPos, 2)));
    QVERIFY(! c.isValidMove(p2KingPiecePos, c.movePos(p2KingPiecePos, downLeftPos, 2)));
    QVERIFY(c.isValidMove(p2KingPiecePos, c.movePos(p2KingPiecePos, upRightPos, 2)));
    QVERIFY(c.isValidMove(p2KingPiecePos, c.movePos(p2KingPiecePos, downRightPos, 2)));
}

void CheckersLogicTests::gameOverTest() {
    c.resetBoard();

    const int player1 = 1;
    const int player2 = 2;

    setBoardP1WinNoP2Pieces(c.board);
    QVERIFY(c.getGameState() == player1);

    c.player1Turn = false;
    setBoardP1WinWithP2Pieces(c.board);
    QVERIFY(c.getGameState() == player1);

    setBoardP2WinNoP1Pieces(c.board);
    QVERIFY(c.getGameState() == player2);

    c.player1Turn = true;
    setBoardP2WinWithP1Pieces(c.board);
    QVERIFY(c.getGameState() == player2);
}

void CheckersLogicTests::emptyBoard(std::vector<std::vector<int>>& board) {
    for(auto& column : board)
        for(auto& square : column)
            square = empty;
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
