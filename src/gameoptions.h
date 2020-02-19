#ifndef GAMEOPTIONS_H
#define GAMEOPTIONS_H

#include <QDialog>

namespace Ui {
class gameOptions;
}

class gameOptions : public QDialog
{
    Q_OBJECT

public:
    explicit gameOptions(QWidget *parent = nullptr);
    ~gameOptions();

private slots:
    void on_start_clicked();

private:
    Ui::gameOptions *ui;
    QString veryEasyDifficulty;
    QString localTwoPlayer;
    QString playervAI;
};

#endif // GAMEOPTIONS_H
