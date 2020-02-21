#include "gameoptions.h"
#include "ui_gameoptions.h"
#include "checkers.h"

extern Checkers* w;

GameOptions::GameOptions(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::gameOptions)
    , veryEasyDifficulty("Very easy")
    , localTwoPlayer("Local PvP")
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

    this->close();
}
