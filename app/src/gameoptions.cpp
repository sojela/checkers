#include "gameoptions.h"
#include "ui_gameoptions.h"
#include "checkers.h"

extern Checkers* MainWindow;

GameOptions::GameOptions(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::gameOptions)
    , veryEasyDifficulty("Very easy")
    , easyDifficulty("Easy")
    , localTwoPlayer("Local 2 player")
    , playervAI("Play against computer")
{
    ui->setupUi(this);

    setWindowTitle("New game");

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    QString ss("background-color: black;"
               "color: white;"
               "selection-background-color: blue;");

    ui->gameType->setStyleSheet(ss);
    ui->difficulty->setStyleSheet(ss);
    ui->start->setStyleSheet(ss);

    ui->gameType->addItem(playervAI);
    ui->gameType->addItem(localTwoPlayer);

    ui->difficulty->addItem(veryEasyDifficulty);
    ui->difficulty->addItem(easyDifficulty);
}

GameOptions::~GameOptions() { delete ui; }

void GameOptions::on_start_clicked() {
    bool isPvP = ui->gameType->currentData(0) == localTwoPlayer;
    if(isPvP)
        MainWindow->typeOfGame = localPvP;
    else
        MainWindow->typeOfGame = PvAI;

    MainWindow->resetBoard();

    this->close();
}

void GameOptions::on_gameType_currentIndexChanged(const QString& arg1) {
    if(arg1 == localTwoPlayer)
        ui->difficulty->hide();
    else
        ui->difficulty->show();
}

void GameOptions::on_difficulty_currentIndexChanged(const QString& arg1) {
    if(arg1 == veryEasyDifficulty)
        MainWindow->difficulty = veryEasy;
    else if(arg1 == easyDifficulty)
        MainWindow->difficulty = easy;
}
