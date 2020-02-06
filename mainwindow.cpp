#include "mainwindow.h"
#include "./ui_mainwindow.h"

enum {empty = 0, player1Piece = 1, player2Piece = 2};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lightSquare(QColor(245, 199, 72))
    , darkSquare(QColor(153, 102, 51))
    , p1Colour(Qt::red)
    , p2Colour(Qt::black)
    , selectedPiece(nullptr)
{
    // set board dimensions to 8x8
    board.resize(8);
    for(int i = 0; i < 8; ++i)
        board[i].resize(8);

    // initial pieces position
    for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {
            if(j < 3 && (i + j) % 2) {
                board[i][j].second = player2Piece;
            } else if(j > 4 && (i + j) % 2) {
                board[i][j].second = player1Piece;
            } else
                board[i][j].second = empty;
        }
    }

    ui->setupUi(this);
}

void MainWindow::paintEvent(QPaintEvent* event) {
    drawBoard();
    drawPieces();
}

void MainWindow::drawBoard() {
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    QSize currentWindowSize = this->size();

    if(windowSize != currentWindowSize) {
        windowSize = currentWindowSize;

        double fractionOfWindowToUse = 0.75;
        int boardLength = std::min(windowSize.height(), windowSize.width()) * fractionOfWindowToUse;
        int startx = (windowSize.width() - boardLength) / 2;
        int starty = (windowSize.height() - boardLength) / 2;
        int gridSquareLength = boardLength / 8;

        for(int i = 0; i < 8; ++i) {
            for(int j = 0; j < 8; ++j) {
                QRect square {startx + (gridSquareLength * i), starty + (gridSquareLength * j), gridSquareLength, gridSquareLength};
                board[i][j].first = square;

                if((i + j) % 2)
                    painter.fillRect(board[i][j].first, darkSquare);
                else
                    painter.fillRect(board[i][j].first, lightSquare);
            }
        }
    }
}

void MainWindow::drawPieces() {
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    QBrush brush {Qt::SolidPattern};
    int pieceDiameter = board[0][0].first.height() * 0.9;

    for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {
            if(board[i][j].second == player1Piece) {
                QRect piece {0, 0, pieceDiameter, pieceDiameter};
                piece.moveCenter(board[i][j].first.center());

                brush.setColor(p1Colour);
                painter.setBrush(brush);
                painter.drawEllipse(piece);
            } else if(board[i][j].second == player2Piece) {
                QRect piece {0, 0, pieceDiameter, pieceDiameter};
                piece.moveCenter(board[i][j].first.center());

                brush.setColor(p2Colour);
                painter.setBrush(brush);
                painter.drawEllipse(piece);
            }
        }
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

std::pair<QRect, int>* MainWindow::findSquare(QPoint pos) {
    for(auto& column : board) {
        for(auto& square : column) {
            if(square.first.contains(pos))
                return &square;
        }
    }

    return nullptr;
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
    if(!(event->button() & Qt::LeftButton))
        return;

    if(!selectedPiece) {
        auto square = findSquare(event->pos());
        if(square && square->second != empty) {
            selectedPiece = square;
            return;
        }
    } else {
        auto square = findSquare(event->pos());
        if(square && square->second == empty) {
            square->second = selectedPiece->second;
            square->first = selectedPiece->first;
            selectedPiece->second = empty;
            selectedPiece = nullptr;
            return;
        }
    }
}
