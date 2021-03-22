#include "gameoptions.h"
#include "ui_gameoptions.h"
#include "checkers.h"

extern Checkers* MainWindow;

GameOptions::GameOptions(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::gameOptions)
    , veryEasyDifficulty("Very easy")
    , easyDifficulty("Easy")
    , normalDifficulty("Normal")
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

    ui->ai->setStyleSheet(ss);
    ui->twoPlayer->setStyleSheet(ss);
    ui->difficulty->setStyleSheet(ss);
    ui->start->setStyleSheet(ss);

    ui->difficulty->addItem(veryEasyDifficulty);
    ui->difficulty->addItem(easyDifficulty);
    ui->difficulty->addItem(normalDifficulty);
}

GameOptions::~GameOptions() { delete ui; }

void GameOptions::on_start_clicked() {
    bool isPvP = ui->twoPlayer->isChecked();
    if(isPvP)
        MainWindow->typeOfGame = localPvP;
    else
        MainWindow->typeOfGame = PvAI;

    MainWindow->resetBoard();

    this->close();
}

void GameOptions::on_difficulty_currentIndexChanged(const QString& arg1) {
    if(arg1 == veryEasyDifficulty)
        MainWindow->difficulty = veryEasy;
    else if(arg1 == easyDifficulty)
        MainWindow->difficulty = easy;
    else if(arg1 == normalDifficulty)
        MainWindow->difficulty = normal;
}

void GameOptions::on_ai_clicked() {
    ui->difficulty->show();
}

void GameOptions::on_twoPlayer_clicked() {
    ui->difficulty->hide();
}
