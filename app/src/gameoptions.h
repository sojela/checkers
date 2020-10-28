#ifndef GAMEOPTIONS_H
#define GAMEOPTIONS_H

#include <QDialog>

namespace Ui { class gameOptions; }

class GameOptions : public QDialog
{
    Q_OBJECT

public:
    explicit GameOptions(QWidget *parent = nullptr);
    ~GameOptions();

private slots:
    void on_start_clicked();
    void on_gameType_currentIndexChanged(const QString &arg1);

private:
    Ui::gameOptions *ui;
    QString veryEasyDifficulty;
    QString localTwoPlayer;
    QString playervAI;
};

#endif // GAMEOPTIONS_H
