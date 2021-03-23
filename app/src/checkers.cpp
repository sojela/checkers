#include "checkers.h"
#include "ui_checkers.h"
#include "gameoptions.h"

#include <QMessageBox>
#include <QFile>
#include <QtConcurrent/QtConcurrentRun>

Checkers::Checkers(QWidget *parent)
    : QMainWindow(parent)
    , number_of_squares_in_board(8)
    , typeOfGame(PvAI)
    , difficulty(veryEasy)
    , ui(new Ui::Checkers)
    , light_square(245, 199, 72)
    , dark_square(153, 102, 51)
    , dark_square_highlight(153, 119, 93)
    , player_1_colour_regular(Qt::red)
    , player_1_colour_king(Qt::darkRed)
    , player_2_colour_regular(Qt::black)
    , player_2_colour_king(Qt::darkGray)
    , background_colour(Qt::black)
    , visible_text_colour(Qt::cyan)
    , mark_colour(Qt::red)
    , square_z_height(0)
    , piece_z_height(1)
    , mark_z_height(2)
    , fractionOfWindowToUse(0.75)
    , pieceSizeFraction(0.9)
    , markSizeFraction(0.1)
    , startedFirstGame(false)
    , gameOver(false)
    , pieceSprites(":/images/checkers.png")
    , invalid_player_two_last_move ({{-1,-1}})
    , playerTwoLastMove(invalid_player_two_last_move)
{
    scene.setBackgroundBrush(background_colour);

    ui->setupUi(this);

    // starting window size
    scene.setSceneRect(0, 0, 800, 600);

    view.setViewport(this);
    view.setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    view.setScene(&scene);
    view.setWindowTitle("Checkers");
    view.show();

    QAction *newGame = new QAction("&New game", this);
    QAction *credits = new QAction("&Credits", this);
    QAction *quit = new QAction("&Quit", this);

    QMenu *file;
    file = menuBar()->addMenu("&File");
    file->addAction(newGame);
    file->addAction(credits);
    file->addSeparator();
    file->addAction(quit);

    connect(newGame, &QAction::triggered, this, &Checkers::startNewGame);
    connect(credits, &QAction::triggered, this, &Checkers::displayCredits);
    connect(quit, &QAction::triggered, this, &Checkers::quit);

    connect(&watcher, SIGNAL(finished()), this, SLOT(aiFinished()));
}

void Checkers::quit() {
    // This is necessary since the AI prevents the program from closing
    exit(0);
}

bool Checkers::isPlayerTwoLastMoveValid() {
    if(playerTwoLastMove[0].x == invalid_player_two_last_move[0].x)
        return false;

    return true;
}

void Checkers::clearMarks() {
    if(isPlayerTwoLastMoveValid())
        for(auto mark : playerTwoLastMove)
            board[mark.x][mark.y].mark = nullptr;

    playerTwoLastMove = invalid_player_two_last_move;
}

void Checkers::resize() {
    if(!startedFirstGame) return;

    int boardLength = std::min(scene.height(), scene.width()) * fractionOfWindowToUse;
    int boardTopLeftCornerX = (scene.width() - boardLength) / 2;
    int boardTopLeftCornerY = (scene.height() - boardLength) / 2;
    int gridSquareLength = boardLength / number_of_squares_in_board;
    double pieceDiameter = gridSquareLength * pieceSizeFraction;

    for(int i = 0; i < number_of_squares_in_board; ++i)
        for(int j = 0; j < number_of_squares_in_board; ++j) {
            auto currentSquare = board[i][j].square;
            currentSquare->setRect(boardTopLeftCornerX + (gridSquareLength * i),
                                   boardTopLeftCornerY + (gridSquareLength * j),
                                   gridSquareLength,
                                   gridSquareLength);

            auto currentPiece = board[i][j].piece;
            if(currentPiece) {
                QRectF boundingRect {0, 0, pieceDiameter, pieceDiameter};
                boundingRect.moveCenter(currentSquare->boundingRect().center());

                currentPiece->setPos(boundingRect.x(), boundingRect.y());

                qreal scale = pieceDiameter / currentPiece->boundingRect().height();
                currentPiece->setScale(scale);
            }
        }


    if(gameOver) updateGameOverTextSizeAndPos();
}

void Checkers::update() {
    if(!startedFirstGame || gameOver) return;

    for(int i = 0; i < number_of_squares_in_board; ++i)
        for(int j = 0; j < number_of_squares_in_board; ++j) {
            bool isLightSquare = (i + j) % 2 == 0;
            if(isLightSquare) continue;

            auto& currentSquare = board[i][j].square;

            auto currentSquareCentre = currentSquare->boundingRect().center();
            auto selectedPieceCentre = board[checkersLogic.getSelectedPiece().x][checkersLogic.getSelectedPiece().y].square->boundingRect().center();
            bool currentSquareSelected = currentSquareCentre == selectedPieceCentre;

            if(checkersLogic.isPieceSelected() && currentSquareSelected)
                currentSquare->setBrush(dark_square_highlight);
            else if(currentSquare->brush() == dark_square_highlight)
                currentSquare->setBrush(dark_square);

            auto& currentPiece = board[i][j].piece;
            if(currentPiece) {
                double pieceDiameter = currentSquare->boundingRect().height() * pieceSizeFraction;
                QRectF boundingRect {0, 0, pieceDiameter, pieceDiameter};
                boundingRect.moveCenter(currentSquare->boundingRect().center());

                currentPiece->setPos(boundingRect.x(), boundingRect.y());
            }
        }

    if(isPlayerTwoLastMoveValid())
        for(auto coord : playerTwoLastMove) {
            auto& currentMark = board[coord.x][coord.y].mark;
            auto& currentSquare = board[coord.x][coord.y].square;

            double markDiameter = currentSquare->boundingRect().height() * markSizeFraction;
            QRectF markBoundingRect {0, 0, markDiameter, markDiameter};
            markBoundingRect.moveCenter(currentSquare->boundingRect().center());

            currentMark = std::shared_ptr<QGraphicsEllipseItem> (new QGraphicsEllipseItem(markBoundingRect));
            scene.addItem(currentMark.get());
            currentMark->setBrush(mark_colour);
            currentMark->setZValue(mark_z_height);
            currentMark->setPen(Qt::NoPen);
        }


    GameState state = checkersLogic.getGameState();

    if(state != current) {
        gameOver = true;

        int winner;
        if(state == player1Wins) {
            winner = 1;
            gameOverSound.setMedia(QUrl("qrc:/sounds/victory.mp3"));
        } else {
            winner = 2;
            gameOverSound.setMedia(QUrl("qrc:/sounds/defeat.mp3"));
        }

        gameOverText.setPlainText("Player " + QString::number(winner) + " wins!");
        updateGameOverTextSizeAndPos();
        gameOverText.setDefaultTextColor(visible_text_colour);

        stopSounds();
        gameOverSound.play();
    }
}

void Checkers::updateGameOverTextSizeAndPos() {
    int fontScale = 20;
    int fontSize = std::min(scene.width(), scene.height()) / fontScale;
    QFont font {"Times", fontSize};
    gameOverText.setFont(font);

    int xPos = (scene.width() / 2) - (gameOverText.boundingRect().width() / 2);
    int yPos = scene.height() * 0.9;
    gameOverText.setPos(xPos, yPos);
}

void Checkers::stopSounds() {
    moveSound.stop();
    kingingSound.stop();
    gameOverSound.stop();
}

Coords Checkers::findPiece(const QPointF& pos) const {
    auto squarePos = findSquare(pos);

    if(board[squarePos.x][squarePos.y].piece)
        return squarePos;
    else
        return {-1, -1};
}

Coords Checkers::findSquare(const QPointF& pos) const {
    auto topLeft = board[0][0].square;
    int x = (pos.x() - topLeft->boundingRect().left()) / topLeft->boundingRect().width();
    int y = (pos.y() - topLeft->boundingRect().top()) / topLeft->boundingRect().height();

    return {x, y};
}

void Checkers::selectPiece(const QPointF& pos) {
    if(gameOver) return;

    bool newPieceWasSelected = checkersLogic.selectPiece(findPiece(pos));
    if(newPieceWasSelected) update();
}

void Checkers::removeCapturedPiece(const Coords& start, const Coords& end) {
    Coords captured;

    if(end.x > start.x)
        captured.x = start.x + 1;
    else
        captured.x = start.x - 1;

    if(end.y > start.y)
        captured.y = start.y + 1;
    else
        captured.y = start.y - 1;

    board[captured.x][captured.y].piece = nullptr;
}

void Checkers::init() {
    // set board dimensions
    board.resize(number_of_squares_in_board);
    for(int i = 0; i < number_of_squares_in_board; ++i)
        board[i].resize(number_of_squares_in_board);

    // create board
    for(int i = 0; i < number_of_squares_in_board; ++i) {
        for(int j = 0; j < number_of_squares_in_board; ++j) {
            auto& currentSquare = board[i][j].square;
            currentSquare = std::shared_ptr<CheckersSquare> (new CheckersSquare);
            currentSquare->setZValue(square_z_height);
            currentSquare->setPen(Qt::NoPen);
            scene.addItem(currentSquare.get());

            if((i + j) % 2)
                currentSquare->setBrush(dark_square);
            else
                currentSquare->setBrush(light_square);
        }
    }


    kingingSound.setMedia(QUrl("qrc:/sounds/kinged.mp3"));
    moveSound.setMedia(QUrl("qrc:/sounds/move.mp3"));

    gameOverText.setDefaultTextColor(background_colour);
    gameOverText.setPlainText("Player 0 wins!");
    scene.addItem(&gameOverText);

    startedFirstGame = true;
}

void Checkers::displayCredits() {
    QFile credits {":/credits.html"};
    if(!credits.open(QFile::ReadOnly | QFile::Text)) return;

    QTextStream in(&credits);
    QString creditsText = in.readAll();

    QMessageBox creditsBox;
    creditsBox.setWindowTitle("Credits");
    creditsBox.setText(creditsText);

    creditsBox.exec();
}

void Checkers::player2AI() {
    if(gameOver) return;

    QFuture<Move> future;

    if(difficulty == veryEasy)
        future = QtConcurrent::run(&ai, &CheckersAI::calculateMoveVeryEasy, checkersLogic);
    else if(difficulty == easy)
        future = QtConcurrent::run(&ai, &CheckersAI::calculateMoveEasy, checkersLogic);
    else if(difficulty == normal)
        future = QtConcurrent::run(&ai, &CheckersAI::calculateMoveNormal, checkersLogic);

    watcher.setFuture(future);
}

void Checkers::kinging(Coords pos) {
    if(checkersLogic.getBoard()[pos.x][pos.y] == player1KingPiece) {
        board[pos.x][pos.y].piece->typeOfPiece = player1KingPiece;
        QRect player1KingPieceSpriteRegion {32, 0, 32, 32};
        QPixmap player1KingPieceSprite = pieceSprites.copy(player1KingPieceSpriteRegion);
        board[pos.x][pos.y].piece->setPixmap(player1KingPieceSprite);
    } else {
        board[pos.x][pos.y].piece->typeOfPiece = player2KingPiece;
        QRect player2KingPieceSpriteRegion {0, 0, 32, 32};
        QPixmap player2KingPieceSprite = pieceSprites.copy(player2KingPieceSpriteRegion);
        board[pos.x][pos.y].piece->setPixmap(player2KingPieceSprite);
    }

    stopSounds();
    kingingSound.play();
}

void Checkers::startNewGame() {
    GameOptions g;
    g.exec();
}

void Checkers::resetBoard() {
    if(!startedFirstGame) init();

    checkersLogic.resetBoard();

    QRect player1SpriteRect {96, 0, 32, 32};
    QPixmap player1Sprite = pieceSprites.copy(player1SpriteRect);

    QRect player2SpriteRect {64, 0, 32, 32};
    QPixmap player2Sprite = pieceSprites.copy(player2SpriteRect);

    for(int i = 0; i < number_of_squares_in_board; ++i) {
        for(int j = 0; j < number_of_squares_in_board; ++j) {
            auto& currentPiece = board[i][j].piece;
            if(checkersLogic.getBoard()[i][j] != empty) {
                currentPiece = std::shared_ptr<CheckersPiece> (new CheckersPiece);
                currentPiece->typeOfPiece = checkersLogic.getBoard()[i][j];

                scene.addItem(currentPiece.get());
                currentPiece->setTransformationMode(Qt::SmoothTransformation);
                if(currentPiece->typeOfPiece == player1Piece)
                    currentPiece->setPixmap(player1Sprite);
                else
                    currentPiece->setPixmap(player2Sprite);
                currentPiece->setZValue(piece_z_height);
            } else
                currentPiece = nullptr;
        }
    }

    clearMarks();

    gameOver = false;
    gameOverText.setDefaultTextColor(background_colour);

    resize();
}

void Checkers::aiFinished() {
    Move move = watcher.result();

    if(!isPlayerTwoLastMoveValid()) {
        playerTwoLastMove.clear();
        playerTwoLastMove.push_back(move.start);
        playerTwoLastMove.push_back(move.destination);
    } else
        playerTwoLastMove.push_back(move.destination);

    selectPiece(board[move.start.x][move.start.y].piece->pos());
    movePiece(board[move.destination.x][move.destination.y].square->boundingRect().center());

    if(checkersLogic.hasCapturedThisTurn())
        player2AI();
    else
        update();
}

void Checkers::movePiece(const QPointF& center) {
    if(!checkersLogic.isPieceSelected()) return;

    auto destination = findSquare(center);
    auto selectedPiece = checkersLogic.getSelectedPiece();

    if(checkersLogic.isValidMove(selectedPiece, destination)) {
        checkersLogic.movePiece(destination);
        board[destination.x][destination.y].piece = board[selectedPiece.x][selectedPiece.y].piece;
        board[selectedPiece.x][selectedPiece.y].piece = nullptr;

        if(board[destination.x][destination.y].piece->typeOfPiece !=
                checkersLogic.getBoard()[destination.x][destination.y])
            kinging(destination);

        if(abs(destination.x - selectedPiece.x) == 2)
            removeCapturedPiece(selectedPiece, destination);

        moveSound.play();

        update();

        if(!checkersLogic.isPlayer1Turn() && typeOfGame == PvAI) {
            if(!checkersLogic.hasCapturedThisTurn())
                clearMarks();
            player2AI();
            update();
        }
    }
}

Checkers::~Checkers() { delete ui; }
