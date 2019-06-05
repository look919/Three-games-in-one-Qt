#include "lobby.h"
#include "ui_lobby.h"
#include <QMessageBox>

Lobby::Lobby(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Lobby)
{
    ui->setupUi(this);

    counter=0;

    //hiding buttons except the first option which is Guess My Word
    hideButtons(ui->startShips);
    hideButtons(ui->startTrainYourMemory);
    hideButtons(ui->rulesbutton);
    hideButtons(ui->authorbutton);
    hideButtons(ui->quitbutton);
    ui->graphiclabel->hide();   //graphic for pressing author purposes

}


void Lobby::on_switchButton_clicked()   //Most important button in lobby
{
    if(counter==0)
    {
        counter++;
        hideButtons(ui->startGuessMyWord);
        showButtons(ui->startShips);
    }
    else if(counter==1)
    {
        counter++;
        hideButtons(ui->startShips);
        showButtons(ui->startTrainYourMemory);
    }
    else if(counter==2)
    {
        counter++;
        hideButtons(ui->startTrainYourMemory);
        showButtons(ui->rulesbutton);
    }
    else if(counter==3)
    {
        counter++;
        hideButtons(ui->rulesbutton);
        showButtons(ui->authorbutton);
    }
    else if(counter==4)
    {
        counter++;
        hideButtons(ui->authorbutton);
        showButtons(ui->quitbutton);
    }
    else
    {
        counter=0;
        hideButtons(ui->quitbutton);
        showButtons(ui->startGuessMyWord);
    }
}

//Games
void Lobby::on_startGuessMyWord_clicked()
{
    Guessmyword newgame(this);
    newgame.setModal(true);
    newgame.exec();
}
void Lobby::on_startTrainYourMemory_clicked()
{
    Trainyourmemory newgame(this);
    newgame.setModal(true);
    newgame.exec();
}
void Lobby::on_startShips_clicked()
{
    Ships newgame(this);
    newgame.setModal(true);
    newgame.exec();
}


void Lobby::on_rulesbutton_clicked()        //rules
{
    Rules newgame(this);
    newgame.setModal(true);
    newgame.exec();
}
void Lobby::on_authorbutton_clicked()       //Highliting info about me
{
    ui->textEdit->setStyleSheet("border: 1px solid red; color:black; background-color:#42f445;");
    ui->textEdit_2->setStyleSheet("border: 1px solid red; color:black; background-color:#42f445;");
    ui->graphiclabel->show();
}
void Lobby::on_quitbutton_clicked()         //Quiting game
{
    this->close();
}



// hide/show buttons
void Lobby::hideButtons(QPushButton *b1)
{
    b1->hide();
}
void Lobby::showButtons(QPushButton *b1)
{
     b1->show();
}

Lobby::~Lobby()                             //Destructor
{
    delete ui;
}


