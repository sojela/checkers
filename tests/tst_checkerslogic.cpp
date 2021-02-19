#include "checkerslogic.h"

#include <QtTest>

class CheckersLogicTests : public QObject, private CheckersLogic {
    Q_OBJECT

private slots:
    void stateAfterResetTest();
    void selectPiecesTest();
    void moveTest();
    void captureTest();

private:
    CheckersLogic c;
};

void CheckersLogicTests::stateAfterResetTest() {
    c.resetBoard();

    QVERIFY(c.isPieceSelected() == false);
    QVERIFY(c.isPlayer1Turn() == true);
    QVERIFY(c.hasCapturedThisTurn() == false);
    QVERIFY(c.getGameState() == current);
}

void CheckersLogicTests::selectPiecesTest() {
    c.resetBoard();

    Coords arbitraryPlayerOnePiece {2, 5};
    Coords arbitraryPlayerTwoPiece {5, 2};

    // player 1
    QVERIFY(c.selectPiece(arbitraryPlayerOnePiece));
    QVERIFY(! c.selectPiece(arbitraryPlayerTwoPiece));

    c.movePiece({3, 4});

    // player 2
    QVERIFY(c.selectPiece(arbitraryPlayerTwoPiece));
    QVERIFY(! c.selectPiece(arbitraryPlayerOnePiece));
}

void CheckersLogicTests::moveTest() {
    c.resetBoard();

    Coords p1Piece = {2, 5};

    QVERIFY(c.isValidMove(p1Piece, c.movePos(p1Piece, upLeftPos)));
    QVERIFY(c.isValidMove(p1Piece, c.movePos(p1Piece, upRightPos)));
    QVERIFY(! c.isValidMove(p1Piece, c.movePos(p1Piece, downLeftPos)));
    QVERIFY(! c.isValidMove(p1Piece, c.movePos(p1Piece, downRightPos)));


    // End player 1's turn
    c.selectPiece(p1Piece);
    c.movePiece({3, 4});


    Coords p2Piece = {3, 2};

    QVERIFY(! c.isValidMove(p2Piece, c.movePos(p2Piece, upLeftPos)));
    QVERIFY(! c.isValidMove(p2Piece, c.movePos(p2Piece, upRightPos)));
    QVERIFY(c.isValidMove(p2Piece, c.movePos(p2Piece, downLeftPos)));
    QVERIFY(c.isValidMove(p2Piece, c.movePos(p2Piece, downRightPos)));
}

void CheckersLogicTests::captureTest() {
    // Prepare board
    c.resetBoard();
    c.selectPiece({2, 5});
    c.movePiece({3, 4});
    c.selectPiece({5, 2});
    c.movePiece({4, 3});


    Coords p1Piece = {3, 4};

    QVERIFY(! c.isValidMove(p1Piece, c.movePos(p1Piece, upLeftPos, 2)));
    QVERIFY(! c.isValidMove(p1Piece, c.movePos(p1Piece, downLeftPos, 2)));
    QVERIFY(c.isValidMove(p1Piece, c.movePos(p1Piece, upRightPos, 2)));
    QVERIFY(! c.isValidMove(p1Piece, c.movePos(p1Piece, downRightPos, 2)));


    // End player 1's turn
    c.selectPiece({0, 5});
    c.movePiece({1, 4});


    Coords p2Piece = {4, 3};

    QVERIFY(! c.isValidMove(p2Piece, c.movePos(p2Piece, upLeftPos, 2)));
    QVERIFY(c.isValidMove(p2Piece, c.movePos(p2Piece, downLeftPos, 2)));
    QVERIFY(! c.isValidMove(p2Piece, c.movePos(p2Piece, upRightPos, 2)));
    QVERIFY(! c.isValidMove(p2Piece, c.movePos(p2Piece, downRightPos, 2)));
}

QTEST_APPLESS_MAIN(CheckersLogicTests)

#include "tst_checkerslogic.moc"
