#include "checkers.h"
#include "ui_checkers.h"

Checkers::Checkers(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Checkers)
    , board_length(8)
    , light_square(245, 199, 72)
    , dark_square(153, 102, 51)
    , dark_square_highlight(153, 119, 93)
    , player_1_colour_regular(Qt::red)
    , player_1_colour_king(Qt::darkRed)
    , player_2_colour_regular(Qt::black)
    , player_2_colour_king(Qt::darkGray)
    , squareZHeight(0)
    , pieceZHeight(1)
    , selectedPiece(-1, -1)
    , pieceSelected(false)
    , player1Turn(true)
{
    // set board dimensions
    board.resize(board_length);
    for(int i = 0; i < board_length; ++i)
        board[i].resize(board_length);

    // create board
    for(auto& column : board) {
        for(auto& square : column) {
            square.first = std::shared_ptr<CheckersSquare> (new CheckersSquare);
            square.first->setZValue(squareZHeight);
            scene.addItem(square.first.get());
        }
    }

    // create pieces and set initial position
    for(int i = 0; i < board_length; ++i) {
        for(int j = 0; j < board_length; ++j) {
            auto& currentPiece = board[i][j].second;
            if(j < 3 && (i + j) % 2) {
                currentPiece = std::shared_ptr<CheckersPiece> (new CheckersPiece);
                currentPiece->typeOfPiece = player2Piece;
                scene.addItem(currentPiece.get());
            } else if(j > 4 && (i + j) % 2) {
                currentPiece = std::shared_ptr<CheckersPiece> (new CheckersPiece);
                currentPiece->typeOfPiece = player1Piece;
                scene.addItem(currentPiece.get());
            } else
                currentPiece = nullptr;

            if(currentPiece)
                currentPiece->setZValue(pieceZHeight);
        }
    }

    scene.addItem(&gameOverText);

    ui->setupUi(this);

    scene.setSceneRect(0, 0, 800, 600);

    view.setViewport(this);
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.setScene(&scene);
    view.show();
}

void Checkers::updateBoard() {
    double fractionOfWindowToUse = 0.75;
    int boardLength = std::min(scene.height(), scene.width()) * fractionOfWindowToUse;
    int startx = (scene.width() - boardLength) / 2;
    int starty = (scene.height() - boardLength) / 2;
    unsigned int gridSquareLength = boardLength / board_length;
    double pieceDiameter = gridSquareLength * 0.9;

    for(int i = 0; i < board_length; ++i)
        for(int j = 0; j < board_length; ++j) {
            auto currentSquare = board[i][j].first;
            currentSquare->setRect(startx + (gridSquareLength * i), starty + (gridSquareLength * j), gridSquareLength, gridSquareLength);
            currentSquare->setPen(Qt::NoPen);

            if((i + j) % 2) {
                if(pieceSelected && currentSquare->boundingRect().center() == board[selectedPiece.first][selectedPiece.second].first->boundingRect().center())
                    currentSquare->setBrush(dark_square_highlight);
                else
                    currentSquare->setBrush(dark_square);
            } else
                currentSquare->setBrush(light_square);

            if(board[i][j].second) {
                auto currentPiece = board[i][j].second;

                if(j == 0 && currentPiece->typeOfPiece == player1Piece)
                    currentPiece->typeOfPiece = player1KingPiece;
                else if(j == 7 && currentPiece->typeOfPiece == player2Piece)
                    currentPiece->typeOfPiece = player2KingPiece;

                QRectF boundingRect {0, 0, pieceDiameter, pieceDiameter};
                boundingRect.moveCenter(board[i][j].first->boundingRect().center());

                currentPiece->setRect(boundingRect);
                currentPiece->setPen(Qt::NoPen);

                switch (currentPiece->typeOfPiece) {
                    case player1Piece:
                        currentPiece->setBrush(player_1_colour_regular);
                        break;
                    case player1KingPiece:
                        currentPiece->setBrush(player_1_colour_king);
                        break;
                    case player2Piece:
                        currentPiece->setBrush(player_2_colour_regular);
                        break;
                    case player2KingPiece:
                        currentPiece->setBrush(player_2_colour_king);
                        break;
                }
            }
        }

    int w = gameOver();

    int fontSize = std::min(scene.width(), scene.height()) / 20;
    QFont font {"Times", fontSize};
    gameOverText.setFont(font);

    gameOverText.setDefaultTextColor(Qt::white);

    gameOverText.setPlainText("Player " + QString::number(w) + " wins!");

    int xPos = (scene.width() / 2) - (gameOverText.boundingRect().width() / 2);
    int yPos = scene.height() * 0.9;
    gameOverText.setPos(xPos, yPos);

    if(w != 0) {
        gameOverText.setDefaultTextColor(Qt::black);
    }
}

bool Checkers::canMove(std::pair<int, int> pos) const {
    std::pair<int, int> upLeft {pos.first - 1, pos.second - 1};
    std::pair<int, int> upRight {pos.first + 1, pos.second - 1};
    std::pair<int, int> downLeft {pos.first - 1, pos.second + 1};
    std::pair<int, int> downRight {pos.first + 1, pos.second + 1};

    if(upLeft.first >= 0 && upLeft.second >= 0) {
        if(isValid(pos, upLeft)) return true;
    }

    if(upRight.first < board_length && upRight.second >= 0) {
        if(isValid(pos, upRight)) return true;
    }

    if(downLeft.first >= 0 && downLeft.second < board_length) {
        if(isValid(pos, downLeft)) return true;
    }

    if(downRight.first < board_length && downRight.second < board_length) {
        if(isValid(pos, downRight)) return true;
    }

    return false;
}

bool Checkers::isMoveable(std::shared_ptr<CheckersPiece> piece) const {
    for(int i = 0; i < board_length; ++i) {
        for(int j = 0; j < board_length; ++j) {
            if(board[i][j].first->boundingRect().center() == piece->boundingRect().center()) {
                if(canCapture({i, j}))
                    return true;

                if(canMove({i, j}))
                    return true;

                return false;
            }
        }
    }

    return false;
}

int Checkers::gameOver() const {
    int countPlayer1Pieces = 0;
    int countPlayer2Pieces = 0;

    for(auto& column : board) {
        for(auto& square : column) {
            if(square.second) {
                if(square.second->typeOfPiece == player1Piece
                    || square.second->typeOfPiece == player1KingPiece)
                    ++countPlayer1Pieces;
                else if(square.second->typeOfPiece == player2Piece
                    || square.second->typeOfPiece == player2KingPiece)
                    ++countPlayer2Pieces;
            }
        }
    }

    if(countPlayer1Pieces == 0)
        return 2;
    else if(countPlayer2Pieces == 0)
        return 1;

    int countPlayer1MoveablePieces = 0;
    int countPlayer2MoveablePieces = 0;

    for(auto& column : board) {
        for(auto& square : column) {
            if(square.second) {
                if((square.second->typeOfPiece == player1Piece || square.second->typeOfPiece == player1KingPiece)
                    && isMoveable(square.second))
                    ++countPlayer1MoveablePieces;
                else if((square.second->typeOfPiece == player2Piece || square.second->typeOfPiece == player2KingPiece)
                    && isMoveable(square.second))
                    ++countPlayer2MoveablePieces;
            }
        }
    }

    if(countPlayer1MoveablePieces == 0 && player1Turn)
        return 2;
    else if(countPlayer2MoveablePieces == 0 && !player1Turn)
        return 1;

    return 0;
}

std::pair<int, int> Checkers::findPiece(QPointF center) const {
    for(int i = 0; i < board_length; ++i) {
        for(int j = 0; j < board_length; ++j) {
            if(!board[i][j].second)
                continue;

            if(board[i][j].first->boundingRect().center() == center)
                return {i, j};
        }
    }

    return {-1, -1};
}

bool Checkers::isCurrentPlayersPiece(std::pair<int, int> pos) const {
    if(player1Turn) {
        if(board[pos.first][pos.second].second->typeOfPiece == player1Piece ||
                board[pos.first][pos.second].second->typeOfPiece == player1KingPiece)
            return true;
    } else {
        if(board[pos.first][pos.second].second->typeOfPiece == player2Piece ||
                board[pos.first][pos.second].second->typeOfPiece == player2KingPiece)
            return true;
    }

    return false;
}

void Checkers::selectPiece(QPointF center) {
    if(hasCapturedThisTurn)
        return;

    auto pos = findPiece(center);
    if(pos.first >= 0) {
        if(!isCurrentPlayersPiece(pos))
            return;

        selectedPiece = pos;
        pieceSelected = true;
        updateBoard();
    }
}

bool Checkers::canCapture(std::pair<int, int> pos) const {
    std::pair<int, int> upLeft {pos.first - 2, pos.second - 2};
    std::pair<int, int> upRight {pos.first + 2, pos.second - 2};
    std::pair<int, int> downLeft {pos.first - 2, pos.second + 2};
    std::pair<int, int> downRight {pos.first + 2, pos.second + 2};

    if(upLeft.first >= 0 && upLeft.second >= 0) {
        if(isValid(pos, upLeft)) return true;
    }

    if(upRight.first < board_length && upRight.second >= 0) {
        if(isValid(pos, upRight)) return true;
    }

    if(downLeft.first >= 0 && downLeft.second < board_length) {
        if(isValid(pos, downLeft)) return true;
    }

    if(downRight.first < board_length && downRight.second < board_length) {
        if(isValid(pos, downRight)) return true;
    }

    return false;
}

void Checkers::removeCapturedPiece(std::pair<int, int> start, std::pair<int, int> end) {
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

void Checkers::movePiece(QPointF center) {
    if(!pieceSelected)
        return;

    for(int i = 0; i < board_length; ++i) {
        for(int j = 0; j < board_length; ++j) {
            if(board[i][j].first->boundingRect().center() == center) {
                if(isValid(selectedPiece, {i, j})) {
                    board[i][j].second = board[selectedPiece.first][selectedPiece.second].second;
                    board[selectedPiece.first][selectedPiece.second].second = nullptr;

                    if(abs(i - selectedPiece.first) == 2) {
                        removeCapturedPiece(selectedPiece, {i, j});
                        hasCapturedThisTurn = true;
                    }

                    if(hasCapturedThisTurn && canCapture({i, j})) {
                        selectedPiece = {i, j};
                        updateBoard();
                        return;
                    }

                    pieceSelected = false;

                    hasCapturedThisTurn = false;

                    if(player1Turn)
                        player1Turn = false;
                    else
                        player1Turn = true;

                    updateBoard();
                }
                return;
            }
        }
    }
}

bool Checkers::canMoveForwards(std::pair<int, int> start, std::pair<int, int> destination) const {
    if(player1Turn) {
        if(start.second - destination.second == 1 && abs(destination.first - start.first) == 1)
            return true;
    } else if(destination.second - start.second == 1 && abs(destination.first - start.first) == 1)
        return true;

    return false;
}

bool Checkers::canMoveBackward(std::pair<int, int> start, std::pair<int, int> destination) const {
    if(player1Turn) {
        if(board[start.first][start.second].second->typeOfPiece == player1KingPiece
            && destination.second - start.second == 1
            && abs(destination.first - start.first) == 1)
            return true;
    } else {
        if(board[start.first][start.second].second->typeOfPiece == player2KingPiece
            && start.second - destination.second == 1
            && abs(destination.first - start.first) == 1)
            return true;
    }

    return false;
}

bool Checkers::canCaptureForwards(std::pair<int, int> start, std::pair<int, int> destination) const {
    if(player1Turn) {
        if(start.second - destination.second == 2 && abs(destination.first - start.first) == 2) {
            std::pair<int, int> enemyPos;
            if(destination.first > start.first)
                enemyPos = {start.first + 1, start.second - 1};
            else
                enemyPos = {start.first - 1, start.second - 1};

            if(!board[enemyPos.first][enemyPos.second].second)
                // no piece to capture
                return false;

            if(!isCurrentPlayersPiece({enemyPos.first, enemyPos.second})) {
                return true;
            }
        }
    } else {
        if(destination.second - start.second == 2 && abs(destination.first - start.first) == 2) {
            std::pair<int, int> enemyPos;
            if(destination.first > start.first)
                enemyPos = {start.first + 1, start.second + 1};
            else
                enemyPos = {start.first - 1, start.second + 1};

            if(!board[enemyPos.first][enemyPos.second].second)
                // no piece to capture
                return false;

            if(!isCurrentPlayersPiece({enemyPos.first, enemyPos.second})) {
                return true;
            }
        }
    }

    return false;
}

bool Checkers::canCaptureBackwards(std::pair<int, int> start, std::pair<int, int> destination) const {
    if(player1Turn) {
        if(board[start.first][start.second].second->typeOfPiece == player1KingPiece) {
            if(destination.second - start.second == 2 && abs(destination.first - start.first) == 2) {
                std::pair<int, int> enemyPos;
                if(destination.first > start.first)
                    enemyPos = {start.first + 1, start.second + 1};
                else
                    enemyPos = {start.first - 1, start.second + 1};

                if(!board[enemyPos.first][enemyPos.second].second)
                    // no piece to capture
                    return false;

                if(!isCurrentPlayersPiece({enemyPos.first, enemyPos.second})) {
                    return true;
                }
            }
        }
    } else {
        if(board[start.first][start.second].second->typeOfPiece == player2KingPiece) {
            if(start.second - destination.second == 2 && abs(destination.first - start.first) == 2) {
                std::pair<int, int> enemyPos;
                if(destination.first > start.first)
                    enemyPos = {start.first + 1, start.second - 1};
                else
                    enemyPos = {start.first - 1, start.second - 1};

                if(!board[enemyPos.first][enemyPos.second].second)
                    // no piece to capture
                    return false;

                if(!isCurrentPlayersPiece({enemyPos.first, enemyPos.second})) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Checkers::isValid(std::pair<int, int> start, std::pair<int, int> destination) const {
    // no piece to move
    if(!board[start.first][start.second].second)
        return false;

    // piece at destination
    if(board[destination.first][destination.second].second)
        return false;

    if(!isCurrentPlayersPiece(start))
        return false;

    if(canMoveForwards(start, destination))
        return true;

    if(canMoveBackward(start, destination))
        return true;

    if(canCaptureForwards(start, destination))
        return true;

    if(canCaptureBackwards(start, destination))
        return true;

    return false;
}

Checkers::~Checkers() {
    delete ui;
}
