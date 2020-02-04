#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lightSquare(QColor(245, 199, 72))
    , darkSquare(QColor(153, 102, 51))
{
    ui->setupUi(this);
}

void MainWindow::paintEvent(QPaintEvent* event) {
    drawBoard();
}

void MainWindow::drawBoard() {
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    QSize window = this->size();

    double fractionOfWindowToUse = 0.75;
    int boardLength = std::min(window.height(), window.width()) * fractionOfWindowToUse;
    int startx = (window.width() - boardLength) / 2;
    int starty = (window.height() - boardLength) / 2;
    int gridSquareLength = boardLength / 8;

    for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {
            QRect square {startx + (gridSquareLength * i), starty + (gridSquareLength * j), gridSquareLength, gridSquareLength};
            painter.drawRect(square);
            if((i + j) % 2)
                painter.fillRect(square, darkSquare);
            else
                painter.fillRect(square, lightSquare);
        }
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

