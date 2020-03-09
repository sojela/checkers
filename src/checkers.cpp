#include "checkers.h"
#include "ui_checkers.h"
#include "gameoptions.h"

#include <QMessageBox>

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
    , text_colour(Qt::cyan)
    , square_z_height(0)
    , piece_z_height(1)
    , button_z_height(2)
    , gameOverText(new QGraphicsTextItem)
    , gameOverSoundPlayed(false)
    , readyToStartFirstGame(false)
    , pieceSprites(":/images/checkers.png")
{
    // set board dimensions
    board.resize(number_of_squares_in_board);
    for(int i = 0; i < number_of_squares_in_board; ++i)
        board[i].resize(number_of_squares_in_board);

    // create board
    for(int i = 0; i < number_of_squares_in_board; ++i) {
        for(int j = 0; j < number_of_squares_in_board; ++j) {
            auto& currentSquare = board[i][j].first;
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

    resetBoard();

    gameOverText->setDefaultTextColor(background_colour);
    scene.addItem(gameOverText);

    scene.setBackgroundBrush(background_colour);

    ui->setupUi(this);

    // starting window size
    scene.setSceneRect(0, 0, 800, 600);

    view.setViewport(this);
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.setScene(&scene);
    view.show();

    QString ss("background-color: #595959;"
               "color: white;"
               "selection-background-color: blue;");
    menuBar()->setStyleSheet(ss);

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
    connect(quit, &QAction::triggered, this, QApplication::quit);

    readyToStartFirstGame = true;
}

void Checkers::update() {
    double fractionOfWindowToUse = 0.75;
    int boardLength = std::min(scene.height(), scene.width()) * fractionOfWindowToUse;
    int boardTopLeftCornerX = (scene.width() - boardLength) / 2;
    int boardTopLeftCornerY = (scene.height() - boardLength) / 2;
    int gridSquareLength = boardLength / number_of_squares_in_board;
    double pieceDiameter = gridSquareLength * 0.9;

    for(int i = 0; i < number_of_squares_in_board; ++i)
        for(int j = 0; j < number_of_squares_in_board; ++j) {
            auto currentSquare = board[i][j].first;
            currentSquare->setRect(boardTopLeftCornerX + (gridSquareLength * i), boardTopLeftCornerY + (gridSquareLength * j), gridSquareLength, gridSquareLength);

            if((i + j) % 2) {
                if(checkersLogic->pieceSelected && currentSquare->boundingRect().center() == board[checkersLogic->selectedPiece.first][checkersLogic->selectedPiece.second].first->boundingRect().center())
                    currentSquare->setBrush(dark_square_highlight);
                else if(currentSquare->brush() == dark_square_highlight)
                    currentSquare->setBrush(dark_square);
            }

            if(board[i][j].second) {
                auto currentPiece = board[i][j].second;

                kinging({i, j});

                QRectF boundingRect {0, 0, pieceDiameter, pieceDiameter};
                boundingRect.moveCenter(board[i][j].first->boundingRect().center());

                currentPiece->setPos(boundingRect.x(), boundingRect.y());

                qreal scale = pieceDiameter / currentPiece->boundingRect().height();
                currentPiece->setScale(scale);
            }
        }

    int winner = checkersLogic->gameOver();

    // this is done every loop so that when the text is displayed it is centred
    int fontSize = std::min(scene.width(), scene.height()) / 20;
    QFont font {"Times", fontSize};
    gameOverText->setFont(font);
    gameOverText->setPlainText("Player " + QString::number(winner) + " wins!");

    int xPos = (scene.width() / 2) - (gameOverText->boundingRect().width() / 2);
    int yPos = scene.height() * 0.9;
    gameOverText->setPos(xPos, yPos);

    if(winner != 0) {
        gameOverText->setDefaultTextColor(text_colour);

        if(!gameOverSoundPlayed && readyToStartFirstGame) {
            if(winner == 1)
                gameOverSound.setMedia(QUrl("qrc:/sounds/victory.mp3"));
            else
                gameOverSound.setMedia(QUrl("qrc:/sounds/defeat.mp3"));

            moveSound.stop();
            kingingSound.stop();

            gameOverSound.play();
            gameOverSoundPlayed = true;
        }
    }
}

std::pair<int, int> Checkers::findPiece(const QPointF& pos) const {
    for(int i = 0; i < number_of_squares_in_board; ++i) {
        for(int j = 0; j < number_of_squares_in_board; ++j) {
            if(!board[i][j].second)
                continue;

            if(board[i][j].first->contains(pos))
                return {i, j};
        }
    }

    return {-1, -1};
}

void Checkers::selectPiece(const QPointF& center) {
    if(checkersLogic->hasCapturedThisTurn)
        return;

    auto pos = findPiece(center);
    if(pos.first >= 0) {
        if(!checkersLogic->isCurrentPlayersPiece(pos))
            return;

        checkersLogic->selectedPiece = pos;
        checkersLogic->pieceSelected = true;
        update();
    }
}

void Checkers::removeCapturedPiece(const std::pair<int, int>& start, const std::pair<int, int>& end) {
    std::pair<int, int> captured;

    if(end.first > start.first)
        captured.first = start.first + 1;
    else
        captured.first = start.first - 1;

    if(end.second > start.second)
        captured.second = start.second + 1;
    else
        captured.second = start.second - 1;

    board[captured.first][captured.second].second = nullptr;
}

void Checkers::displayCredits() {
    QMessageBox creditsBox;
    creditsBox.setWindowTitle("Credits");

    QVector<QVector<QString>> soundCredits;

    soundCredits.push_back({"https://freesound.org/people/simone_ds/sounds/366065/",
                            "chess pieces.wav",
                            "https://freesound.org/people/simone_ds/",
                            "simone_ds",
                            "https://creativecommons.org/publicdomain/zero/1.0/",
                            "CC0 1.0"});

    soundCredits.push_back({"https://freesound.org/people/qubodup/sounds/442943/",
                            "Level Up",
                            "https://freesound.org/people/qubodup/",
                            "qubodup",
                            "https://creativecommons.org/publicdomain/zero/1.0/",
                            "CC0 1.0"});

    soundCredits.push_back({"https://freesound.org/people/Kubatko/sounds/336725/",
                            "Inception Horn Victory",
                            "https://freesound.org/people/Kubatko/",
                            "Kubatko",
                            "https://creativecommons.org/publicdomain/zero/1.0/",
                            "CC0 1.0"});

    soundCredits.push_back({"https://freesound.org/people/Leszek_Szary/sounds/133283/",
                            "game over",
                            "https://freesound.org/people/Leszek_Szary/",
                            "Leszek_Szary",
                            "https://creativecommons.org/publicdomain/zero/1.0/",
                            "CC0 1.0"});

    QVector<QVector<QString>> imageCredits;

    imageCredits.push_back({"https://opengameart.org/content/checkers",
                            "Checkers",
                            "https://opengameart.org/users/andi",
                            "Andi Peredri",
                            "https://creativecommons.org/licenses/by/3.0/",
                            "CC BY 3.0"});

    QString creditsText = "<p>Created by Suraj Ojela</p>";

    creditsText += "<p>Sounds</p>";

    for(auto& c : soundCredits) {
        creditsText += "<a href='"
                + c[0]
                + "'>\""
                + c[1]
                + "\"</a> by <a href='"
                + c[2]
                + "'>"
                + c[3]
                + "</a>\", licensed under <a href='"
                + c[4]
                + "'>"
                + c[5]
                + "</a> <br>";
    }

    creditsText += "<p>Images</p>";

    for(auto& c : imageCredits) {
        if(c.size() > 1)
            creditsText += "<div><a href='"
                    + imageCredits[0][0]
                    + "'>\""
                    + imageCredits[0][1]
                    + "\"</a> by <a href='"
                    + imageCredits[0][2]
                    + "'>"
                    + imageCredits[0][3]
                    + "</a>\", licensed under <a href='"
                    + imageCredits[0][4]
                    + "'>"
                    + imageCredits[0][5]
                    + "</a><div>";
        else
            creditsText += c[0]
                    + "<br>";
    }

    creditsBox.setText(creditsText);

    creditsBox.exec();
}

void Checkers::endTurn() {
    checkersLogic->endTurn();

    if(!checkersLogic->player1Turn && typeOfGame == PvAI)
        player2AI();
}

void Checkers::player2AI() {
    if(checkersLogic->gameOver())
        return;

    std::pair<std::pair<int, int>, std::pair<int, int>> move;

    if(difficulty == veryEasy) {
        move = ai.calculateMoveVeryEasy(*checkersLogic);
        selectPiece(QPointF(board[move.first.first][move.first.second].second->x(),
                            board[move.first.first][move.first.second].second->y()));
        movePiece(board[move.second.first][move.second.second].first->boundingRect().center());

        while(checkersLogic->hasCapturedThisTurn) {
            move = ai.calculateMoveVeryEasy(*checkersLogic);
            selectPiece(QPointF(board[move.first.first][move.first.second].second->x(),
                                board[move.first.first][move.first.second].second->y()));
            movePiece(board[move.second.first][move.second.second].first->boundingRect().center());
        }
    }
}

void Checkers::kinging(std::pair<int, int> pos) {
    if(checkersLogic->kinging(pos)) {
        board[pos.first][pos.second].second->typeOfPiece = checkersLogic->board[pos.first][pos.second];

        if(board[pos.first][pos.second].second->typeOfPiece == player1KingPiece) {
            QRect rect {32, 0, 32, 32};
            QPixmap cropped = pieceSprites.copy(rect);
            board[pos.first][pos.second].second->setPixmap(cropped);
        } else {
            QRect rect {0, 0, 32, 32};
            QPixmap cropped = pieceSprites.copy(rect);
            board[pos.first][pos.second].second->setPixmap(cropped);
        }

        kingingSound.play();
    }
}

void Checkers::startNewGame() {
    GameOptions g;
    g.exec();
}

void Checkers::resetBoard() {
    QRect player1SpriteRect {96, 0, 32, 32};
    QPixmap player1Sprite = pieceSprites.copy(player1SpriteRect);

    QRect player2SpriteRect {64, 0, 32, 32};
    QPixmap player2Sprite = pieceSprites.copy(player2SpriteRect);

    for(int i = 0; i < number_of_squares_in_board; ++i) {
        for(int j = 0; j < number_of_squares_in_board; ++j) {
            auto& currentPiece = board[i][j].second;
            if(j < 3 && (i + j) % 2) {
                currentPiece = std::shared_ptr<CheckersPiece> (new CheckersPiece);
                currentPiece->typeOfPiece = player2Piece;
                scene.addItem(currentPiece.get());
                currentPiece->setPixmap(player2Sprite);
                currentPiece->setTransformationMode(Qt::SmoothTransformation);
            } else if(j > 4 && (i + j) % 2) {
                currentPiece = std::shared_ptr<CheckersPiece> (new CheckersPiece);
                currentPiece->typeOfPiece = player1Piece;
                scene.addItem(currentPiece.get());
                currentPiece->setPixmap(player1Sprite);
                currentPiece->setTransformationMode(Qt::SmoothTransformation);
            } else
                currentPiece = nullptr;

            if(currentPiece)
                currentPiece->setZValue(piece_z_height);
        }
    }

    delete checkersLogic;
    checkersLogic = new CheckersLogic(number_of_squares_in_board);
    checkersLogic->setBoard(board);

    gameOverSoundPlayed = false;
    gameOverText->setDefaultTextColor(background_colour);

    update();
}

void Checkers::movePiece(const QPointF& center) {
    if(!checkersLogic->pieceSelected)
        return;

    for(int i = 0; i < number_of_squares_in_board; ++i) {
        for(int j = 0; j < number_of_squares_in_board; ++j) {
            if(board[i][j].first->boundingRect().center() == center) {
                auto selectedPiece = checkersLogic->selectedPiece;
                if(checkersLogic->movePiece({i, j})) {
                    board[i][j].second = board[selectedPiece.first][selectedPiece.second].second;
                    board[selectedPiece.first][selectedPiece.second].second = nullptr;

                    if(abs(i - selectedPiece.first) == 2)
                        removeCapturedPiece(selectedPiece, {i, j});

                    moveSound.play();

                    if(checkersLogic->hasCapturedThisTurn && checkersLogic->canCapture({i, j})) {
                        update();
                        return;
                    } else
                        endTurn();

                    update();
                }
            }
        }
    }
}

Checkers::~Checkers() {
    delete checkersLogic;
    delete ui;
}
