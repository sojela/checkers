#ifndef CHECKERS_H
#define CHECKERS_H

#include "checkerssquare.h"
#include "checkersview.h"
#include "checkerspiece.h"
#include "checkersai.h"

#include <QMainWindow>
#include <QVector>
#include <QGraphicsScene>
#include <QMediaPlayer>

enum TypeOfGame {localPvP, PvAI};
enum Difficulty {veryEasy};

QT_BEGIN_NAMESPACE
namespace Ui { class Checkers; }
QT_END_NAMESPACE

class Checkers : public QMainWindow
{
    Q_OBJECT

public:
    Checkers(QWidget *parent = nullptr);
    ~Checkers();
    void selectPiece(QPointF center);
    void movePiece(QPointF center);
    void updateBoard();
    bool isMoveable(std::shared_ptr<CheckersPiece> piece) const;
    bool isValid(std::pair<int, int> start, std::pair<int, int> destination) const;
    void startNewGame();

    const int board_length;
    bool hasCapturedThisTurn;
    std::pair<int, int> selectedPiece;
    TypeOfGame typeOfGame;
    Difficulty difficulty;

private slots:
    void resetBoard();

private:
    std::pair<int, int> findPiece(QPointF center) const;
    bool isCurrentPlayersPiece(std::pair<int, int> pos) const;
    bool canMoveForwards(std::pair<int, int> start, std::pair<int, int> destination) const;
    bool canMoveBackward(std::pair<int, int> start, std::pair<int, int> destination) const;
    bool canCaptureForwards(std::pair<int, int> start, std::pair<int, int> destination) const;
    bool canCaptureBackwards(std::pair<int, int> start, std::pair<int, int> destination) const;
    bool canCapture(std::pair<int, int> pos) const;
    bool canMove(std::pair<int, int> pos) const;
    int gameOver() const;
    void removeCapturedPiece(std::pair<int, int> start, std::pair<int, int> end);
    void displayCredits();
    void endTurn();
    void player2AI();

    Ui::Checkers *ui;
    const QColor light_square;
    const QColor dark_square;
    const QColor dark_square_highlight;
    const QColor player_1_colour_regular;
    const QColor player_1_colour_king;
    const QColor player_2_colour_regular;
    const QColor player_2_colour_king;
    const QColor background_colour;
    const QColor text_colour;
    const qreal square_z_height;
    const qreal piece_z_height;
    QVector<QVector<std::pair<std::shared_ptr<CheckersSquare>, std::shared_ptr<CheckersPiece>>>> board;
    std::pair<int, int> captured;
    bool pieceSelected;
    bool player1Turn;
    QGraphicsScene scene;
    CheckersView view;
    QGraphicsTextItem gameOverText;
    QMediaPlayer moveSound;
    QMediaPlayer kingingSound;
    QMediaPlayer gameOverSound;
    bool gameOverSoundPlayed;
    bool startFirstGame;
    CheckersAI ai;
    QPixmap pieceSprites;

};
#endif // CHECKERS_H
