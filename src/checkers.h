#ifndef CHECKERS_H
#define CHECKERS_H

#include "checkersview.h"
#include "checkersai.h"
#include "checkerslogic.h"

#include <QMainWindow>
#include <QVector>
#include <QGraphicsScene>
#include <QMediaPlayer>

enum TypeOfGame {localPvP, PvAI};
enum Difficulty {veryEasy};

QT_BEGIN_NAMESPACE
namespace Ui { class Checkers; }
QT_END_NAMESPACE

class Checkers : public QMainWindow {
    Q_OBJECT

public:
    Checkers(QWidget *parent = nullptr);
    ~Checkers();
    void selectPiece(const QPointF& center);
    void movePiece(const QPointF& center);
    void update();
    void displayCredits();

    void startNewGame();
    void resetBoard();

    const int number_of_squares_in_board;
    TypeOfGame typeOfGame;
    Difficulty difficulty;

private:
    std::pair<int, int> findPiece(const QPointF& pos) const;
    void endTurn();
    void player2AI();
    void kinging(std::pair<int, int> pos);
    void removeCapturedPiece(const std::pair<int, int>& start, const std::pair<int, int>& end);
    void init();

    Ui::Checkers* ui;
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
    QGraphicsScene scene;
    CheckersView view;
    QGraphicsTextItem* gameOverText;
    QMediaPlayer* moveSound;
    QMediaPlayer* kingingSound;
    QMediaPlayer* gameOverSound;
    bool gameOverSoundPlayed;
    bool startedFirstGame;
    CheckersAI* ai;
    QPixmap* pieceSprites;
    CheckersLogic* checkersLogic;
};
#endif // CHECKERS_H
