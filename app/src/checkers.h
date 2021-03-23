#ifndef CHECKERS_H
#define CHECKERS_H

#include "checkerspiece.h"
#include "checkersview.h"
#include "checkersai.h"
#include "checkerslogic.h"
#include "types.h"

#include <QMainWindow>
#include <QVector>
#include <QGraphicsScene>
#include <QMediaPlayer>
#include <QFutureWatcher>
#include <vector>

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
    void resize();
    void displayCredits();
    void startNewGame();
    void resetBoard();

    const int number_of_squares_in_board;
    TypeOfGame typeOfGame;
    Difficulty difficulty;

private slots:
    void aiFinished();

private:
    Coords findPiece(const QPointF& pos) const;
    Coords findSquare(const QPointF& pos) const;
    void player2AI();
    void kinging(Coords pos);
    void removeCapturedPiece(const Coords& start, const Coords& end);
    void init();
    void update();
    void updateGameOverTextSizeAndPos();
    void stopSounds();
    void quit();
    bool isPlayerTwoLastMoveValid();
    void clearMarks();

    struct BoardSquare {
        std::shared_ptr<CheckersSquare> square;
        std::shared_ptr<CheckersPiece> piece;
        std::shared_ptr<QGraphicsEllipseItem> mark;
    };

    QVector<QVector<BoardSquare>> board;
    Ui::Checkers* ui;
    const QColor light_square;
    const QColor dark_square;
    const QColor dark_square_highlight;
    const QColor player_1_colour_regular;
    const QColor player_1_colour_king;
    const QColor player_2_colour_regular;
    const QColor player_2_colour_king;
    const QColor background_colour;
    const QColor visible_text_colour;
    const QColor mark_colour;
    const qreal square_z_height;
    const qreal piece_z_height;
    const qreal mark_z_height;
    const double fractionOfWindowToUse;
    const double pieceSizeFraction;
    const double markSizeFraction;
    QGraphicsScene scene;
    CheckersView view;
    QGraphicsTextItem gameOverText;
    QMediaPlayer moveSound;
    QMediaPlayer kingingSound;
    QMediaPlayer gameOverSound;
    bool startedFirstGame;
    bool gameOver;
    CheckersAI ai;
    QPixmap pieceSprites;
    CheckersLogic checkersLogic;
    QFutureWatcher<Move> watcher;
    const std::vector<Coords> invalid_player_two_last_move;
    std::vector<Coords> playerTwoLastMove;
};
#endif // CHECKERS_H
