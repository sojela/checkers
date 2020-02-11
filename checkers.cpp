#include "checkers.h"
#include "ui_checkers.h"

Checkers::Checkers(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Checkers)
    , board_length(8)
    , light_square(245, 199, 72)
    , dark_square(153, 102, 51)
    , player_1_colour(Qt::red)
    , player_2_colour(Qt::black)
    , squareZHeight(0)
    , pieceZHeight(1)
    , selectedPiece(-1, -1)
    , pieceSelected(false)
{
    // set board dimensions
    board.resize(board_length);
    for(int i = 0; i < board_length; ++i)
        board[i].resize(board_length);

    // create board
    for(auto& column : board) {
        for(auto& square : column) {
            square.first = std::shared_ptr<CheckersSquare> (new CheckersSquare);
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

            currentSquare->setZValue(squareZHeight);

            if((i + j) % 2)
                currentSquare->setBrush(dark_square);
            else
                currentSquare->setBrush(light_square);

            if(board[i][j].second) {
                auto currentPiece = board[i][j].second;
                currentPiece->setZValue(pieceZHeight);

                QRectF boundingRect {0, 0, pieceDiameter, pieceDiameter};
                boundingRect.moveCenter(board[i][j].first->boundingRect().center());

                currentPiece->setRect(boundingRect);
                currentPiece->setPen(Qt::NoPen);

                if(currentPiece->typeOfPiece == player1Piece || currentPiece->typeOfPiece == player1KingPiece)
                    currentPiece->setBrush(player_1_colour);
                else if(currentPiece->typeOfPiece == player2Piece || currentPiece->typeOfPiece == player2KingPiece)
                    currentPiece->setBrush(player_2_colour);
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

void Checkers::selectPiece(QPointF center) {
    if(pieceSelected)
        return;

    auto pos = findPiece(center);
    if(pos.first >= 0) {
        selectedPiece = pos;
        pieceSelected = true;
    }
}

void Checkers::movePiece(QPointF center) {
    if(!pieceSelected)
        return;

    for(int i = 0; i < board_length; ++i) {
        for(int j = 0; j < board_length; ++j) {
            if(board[i][j].first->boundingRect().center() == center) {
                if(!board[i][j].second) {
                    board[i][j].second = board[selectedPiece.first][selectedPiece.second].second;
                    board[selectedPiece.first][selectedPiece.second].second = nullptr;
                    pieceSelected = false;
                    updateBoard();
                }
                return;
            }
        }
    }
}

Checkers::~Checkers() {
    delete ui;
}
