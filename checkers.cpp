#include "checkers.h"
#include "ui_checkers.h"

Checkers::Checkers(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Checkers)
    , board_length(8)
    , light_square(245, 199, 72)
    , light_square_highlight(245, 209, 111)
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
            } else {
                if(pieceSelected && currentSquare->boundingRect().center() == board[selectedPiece.first][selectedPiece.second].first->boundingRect().center())
                    currentSquare->setBrush(light_square_highlight);
                else
                    currentSquare->setBrush(light_square);
            }

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
}

std::pair<int, int> Checkers::findPiece(QPointF center) {
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

bool Checkers::isCurrentPlayersPiece(std::pair<int, int> pos) {
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
    auto pos = findPiece(center);
    if(pos.first >= 0) {
        if(!isCurrentPlayersPiece(pos))
            return;

        selectedPiece = pos;
        pieceSelected = true;
        updateBoard();
    }
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

                    for(auto& pos : captured)
                        board[pos.first][pos.second].second = nullptr;
                    captured.clear();

                    pieceSelected = false;

                    updateBoard();

                    if(player1Turn)
                        player1Turn = false;
                    else
                        player1Turn = true;
                }
                return;
            }
        }
    }
}

bool Checkers::moveForwards(std::pair<int, int> start, std::pair<int, int> destination) {
    if(player1Turn) {
        if(start.second - destination.second == 1 && abs(destination.first - start.first) == 1)
            return true;
    } else if(destination.second - start.second == 1 && abs(destination.first - start.first) == 1)
        return true;

    return false;
}

bool Checkers::moveBackwards(std::pair<int, int> start, std::pair<int, int> destination) {
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

bool Checkers::captureForwards(std::pair<int, int> start, std::pair<int, int> destination) {
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
                captured.push_back(enemyPos);
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
                captured.push_back(enemyPos);
                return true;
            }
        }
    }

    return false;
}

bool Checkers::captureBackwards(std::pair<int, int> start, std::pair<int, int> destination) {
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
                    captured.push_back(enemyPos);
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
                    captured.push_back(enemyPos);
                    return true;
                }
            }
        }
    }

    return false;
}

bool Checkers::isValid(std::pair<int, int> start, std::pair<int, int> destination) {
    // no piece to move
    if(!board[start.first][start.second].second)
        return false;

    // piece at destination
    if(board[destination.first][destination.second].second)
        return false;

    if(!isCurrentPlayersPiece(start))
        return false;

    if(moveForwards(start, destination))
        return true;

    if(moveBackwards(start, destination))
        return true;

    if(captureForwards(start, destination))
        return true;

    if(captureBackwards(start, destination))
        return true;

    return false;
}

Checkers::~Checkers() {
    delete ui;
}
