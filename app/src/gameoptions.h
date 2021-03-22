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
    void on_difficulty_currentIndexChanged(const QString &arg1);
    void on_ai_clicked();
    void on_twoPlayer_clicked();

private:
    Ui::gameOptions *ui;
    QString veryEasyDifficulty;
    QString easyDifficulty;
    QString normalDifficulty;
};

#endif // GAMEOPTIONS_H
