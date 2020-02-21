#include "gameoptions.h"
#include "ui_gameoptions.h"
#include "checkers.h"

extern Checkers* w;

GameOptions::GameOptions(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::gameOptions)
    , veryEasyDifficulty("Very easy")
    , localTwoPlayer("Local 2 player")
    , playervAI("Play against computer")
{
    ui->setupUi(this);

    ui->gameType->addItem(playervAI);
    ui->gameType->addItem(localTwoPlayer);

    ui->difficulty->addItem(veryEasyDifficulty);
}

GameOptions::~GameOptions() {
    delete ui;
}

void GameOptions::on_start_clicked() {
    if(ui->gameType->currentData(0) == localTwoPlayer)
        w->typeOfGame = localPvP;
    else
        w->typeOfGame = PvAI;

    w->difficulty = veryEasy;

    w->resetBoard();

    this->close();
}

void GameOptions::on_gameType_currentIndexChanged(const QString& arg1) {
    if(arg1 == localTwoPlayer)
        ui->difficulty->hide();
    else
        ui->difficulty->show();
}
