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
    void selectPiece(const QPointF& center);
    void movePiece(const QPointF& center);
    void update();
    bool isMoveable(const std::shared_ptr<CheckersPiece>& piece) const;
    bool isValid(const std::pair<int, int>& start, const std::pair<int, int>& destination) const;
    void startNewGame();
    void resetBoard();
    void displayCredits();

    const int number_of_squares_in_board;
    bool hasCapturedThisTurn;
    std::pair<int, int> selectedPiece;
    TypeOfGame typeOfGame;
    Difficulty difficulty;

private:
    std::pair<int, int> findPiece(const QPointF& center) const;
    bool isCurrentPlayersPiece(const std::pair<int, int>& pos) const;
    bool canMoveForwardsWithoutCapture(const std::pair<int, int>& start, const std::pair<int, int>& destination) const;
    bool canMoveBackwardWithoutCapture(const std::pair<int, int>& start, const std::pair<int, int>& destination) const;
    bool canCaptureForwards(const std::pair<int, int>& start, const std::pair<int, int>& destination) const;
    bool canCaptureBackwards(const std::pair<int, int>& start, const std::pair<int, int>& destination) const;
    bool canCapture(const std::pair<int, int>& pos) const;
    bool canMoveWithoutCapture(const std::pair<int, int>& pos) const;
    int gameOver() const;
    void removeCapturedPiece(const std::pair<int, int>& start, const std::pair<int, int>& end);
    void endTurn();
    void player2AI();
    void kinging(int height, std::shared_ptr<CheckersPiece> piece);

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
    const qreal button_z_height;
    QVector<QVector<std::pair<std::shared_ptr<CheckersSquare>, std::shared_ptr<CheckersPiece>>>> board;
    std::pair<int, int> captured;
    bool pieceSelected;
    bool player1Turn;
    QGraphicsScene scene;
    CheckersView view;
    QGraphicsTextItem* gameOverText;
    QMediaPlayer moveSound;
    QMediaPlayer kingingSound;
    QMediaPlayer gameOverSound;
    bool gameOverSoundPlayed;
    bool readyToStartFirstGame;
    CheckersAI ai;
    QPixmap pieceSprites;
};
#endif // CHECKERS_H
