#ifndef LOBBY_H
#define LOBBY_H

#include <QWidget>
#include <QPushButton>
#include "trainyourmemory.h"

namespace Ui {
class Lobby;
}

class Lobby : public QWidget
{
    Q_OBJECT

public:
    explicit Lobby(QWidget *parent = nullptr);
    ~Lobby();

private slots:

    void on_switchButton_clicked();         //Switching buttons in lobby
    void on_startGuessMyWord_clicked();     //games
    void on_startTrainYourMemory_clicked();
    void on_startShips_clicked();

    void on_rulesbutton_clicked();          //rest option in lobby
    void on_authorbutton_clicked();
    void on_quitbutton_clicked();

    void hideButtons(QPushButton *b1);       //hide/showbuttons
    void showButtons(QPushButton *b1);


private:
    Ui::Lobby *ui;
    int counter;
};

#endif // LOBBY_H
