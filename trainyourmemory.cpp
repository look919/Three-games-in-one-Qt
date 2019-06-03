#include "trainyourmemory.h"
#include "ui_trainyourmemory.h"
#include <QDebug>
#include <windows.h>
#include <QTime>

Trainyourmemory::Trainyourmemory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Trainyourmemory)
{
    ui->setupUi(this);

    srand((static_cast<unsigned int>(time(nullptr))));
    playGame=false;
}

void Trainyourmemory::startRound()
{
    delay(1);
    QString roundNr=QString::number(roundCounter);
    QString setRoundNrString=QString("Round nr %1").arg(roundNr);
    ui->commentator->setText(setRoundNrString);

    delay(2);
    buttons.clear();          //deleting elements from vector before each round

    int redColorFrequency=7;
    if(roundCounter>=6) redColorFrequency=6;
    if(roundCounter>=8) redColorFrequency=5;
    if(roundCounter>=10) redColorFrequency=4;

    disable_buttons();
    for(int i=0;i<roundCounter+2;i++)
    {

      int random=rand()%redColorFrequency;

      if(roundCounter>=4)        //after 3 rounds there is possibility of drawing button on red
      {
            if(random==0)
            {
                i-=1;           //if its fake color then dont change iterator
                paintRandomButtonOnRed();
                delay(1);
                cleanButtons();
                delay(1);
            }
            else
            {
                paintRandomButtonOnGreen();
                delay(1);
                cleanButtons();
                delay(1);
            }
      }

      else                  //before 4th round its only green color
      {
          paintRandomButtonOnGreen();
          delay(1);
          cleanButtons();
          delay(1);
      }
    }
    ui->commentator->setText("Go on!");
    enable_buttons();

    index=0;                     //setting index on each round
    lastelement=buttons.size()-1;  //setting last element

}

void Trainyourmemory::on_anybutton_clicked(QPushButton *n)
{
     if(playGame)
    {
        cleanButtons();
        if(index<=lastelement)       //if index is not bigger than the last one
        {
            clickedButtonName=n->objectName();
            if(clickedButtonName==buttons.at(index))
            {
                n->setStyleSheet("background-color:#204f23");
                index++;
            }
            else
            {
                mistake();
            }
        }

        if(index>lastelement)
        {
            roundCounter++;
            wonround();
            delay(2);
            cleanButtons();
            startRound();
        }
    }
}

void Trainyourmemory::on_button1_clicked()
{
    on_anybutton_clicked(ui->button1);
}

void Trainyourmemory::on_button2_clicked()
{
    on_anybutton_clicked(ui->button2);
}

void Trainyourmemory::on_button3_clicked()
{
    on_anybutton_clicked(ui->button3);
}

void Trainyourmemory::on_button4_clicked()
{
    on_anybutton_clicked(ui->button4);
}

void Trainyourmemory::on_button5_clicked()    //most important button that starts the game
{
    if(!playGame)
    {
        playGame=1;
        roundCounter=1;

        cleanButtons();
        ui->button5->setText("");
        ui->commentator->clear();
        startRound();
    }
    else        //thats the place that is similar to all buttons
    {
        on_anybutton_clicked(ui->button5);
    }
}

void Trainyourmemory::on_button6_clicked()
{
    on_anybutton_clicked(ui->button6);
}

void Trainyourmemory::on_button7_clicked()
{
    on_anybutton_clicked(ui->button7);
}

void Trainyourmemory::on_button8_clicked()
{
    on_anybutton_clicked(ui->button8);
}

void Trainyourmemory::on_button9_clicked()
{
    on_anybutton_clicked(ui->button9);
}

void Trainyourmemory::paintRandomButtonOnGreen()
{
    int randomButton=rand()%9+1;
    QString randomButtonName = "button"+QString::number(randomButton);

    if     (randomButtonName==ui->button1->objectName())  ui->button1->setStyleSheet("background-color:#0dbc33;");
    else if(randomButtonName==ui->button2->objectName())  ui->button2->setStyleSheet("background-color:#0dbc33;");
    else if(randomButtonName==ui->button3->objectName())  ui->button3->setStyleSheet("background-color:#0dbc33;");
    else if(randomButtonName==ui->button4->objectName())  ui->button4->setStyleSheet("background-color:#0dbc33;");
    else if(randomButtonName==ui->button5->objectName())  ui->button5->setStyleSheet("background-color:#0dbc33;");
    else if(randomButtonName==ui->button6->objectName())  ui->button6->setStyleSheet("background-color:#0dbc33;");
    else if(randomButtonName==ui->button7->objectName())  ui->button7->setStyleSheet("background-color:#0dbc33;");
    else if(randomButtonName==ui->button8->objectName())  ui->button8->setStyleSheet("background-color:#0dbc33;");
    else if(randomButtonName==ui->button9->objectName())  ui->button9->setStyleSheet("background-color:#0dbc33;");

    buttons.push_back(randomButtonName);
}
void Trainyourmemory::paintRandomButtonOnRed()
{
    int randomButton=rand()%9+1;
    QString randomButtonName = "button"+QString::number(randomButton);

    if     (randomButtonName==ui->button1->objectName())  ui->button1->setStyleSheet("background-color:#bc0d15;");
    else if(randomButtonName==ui->button2->objectName())  ui->button2->setStyleSheet("background-color:#bc0d15;");
    else if(randomButtonName==ui->button3->objectName())  ui->button3->setStyleSheet("background-color:#bc0d15;");
    else if(randomButtonName==ui->button4->objectName())  ui->button4->setStyleSheet("background-color:#bc0d15;");
    else if(randomButtonName==ui->button5->objectName())  ui->button5->setStyleSheet("background-color:#bc0d15;");
    else if(randomButtonName==ui->button6->objectName())  ui->button6->setStyleSheet("background-color:#bc0d15;");
    else if(randomButtonName==ui->button7->objectName())  ui->button7->setStyleSheet("background-color:#bc0d15;");
    else if(randomButtonName==ui->button8->objectName())  ui->button8->setStyleSheet("background-color:#bc0d15;");
    else if(randomButtonName==ui->button9->objectName())  ui->button9->setStyleSheet("background-color:#bc0d15;");
}
void Trainyourmemory::cleanButtons()
{
    ui->button1->setStyleSheet("");
    ui->button2->setStyleSheet("");
    ui->button3->setStyleSheet("");
    ui->button4->setStyleSheet("");
    ui->button5->setStyleSheet("");
    ui->button6->setStyleSheet("");
    ui->button7->setStyleSheet("");
    ui->button8->setStyleSheet("");
    ui->button9->setStyleSheet("");
}

void Trainyourmemory::wonround()
{
    ui->button1->setStyleSheet("background-color:#204f23;");
    ui->button2->setStyleSheet("background-color:#204f23;");
    ui->button3->setStyleSheet("background-color:#204f23;");
    ui->button4->setStyleSheet("background-color:#204f23;");
    ui->button5->setStyleSheet("background-color:#204f23;");    //other color for button5
    ui->button6->setStyleSheet("background-color:#204f23;");
    ui->button7->setStyleSheet("background-color:#204f23;");
    ui->button8->setStyleSheet("background-color:#204f23;");
    ui->button9->setStyleSheet("background-color:#204f23;");
    ui->commentator->setText("Good Job!");
}
void Trainyourmemory::mistake()
{
    if     (clickedButtonName==ui->button1->objectName())  ui->button1->setStyleSheet("background-color:#bc0d15;");
    else if(clickedButtonName==ui->button2->objectName())  ui->button2->setStyleSheet("background-color:#bc0d15;");
    else if(clickedButtonName==ui->button3->objectName())  ui->button3->setStyleSheet("background-color:#bc0d15;");
    else if(clickedButtonName==ui->button4->objectName())  ui->button4->setStyleSheet("background-color:#bc0d15;");
    else if(clickedButtonName==ui->button5->objectName())  ui->button5->setStyleSheet("background-color:#bc0d15;");
    else if(clickedButtonName==ui->button6->objectName())  ui->button6->setStyleSheet("background-color:#bc0d15;");
    else if(clickedButtonName==ui->button7->objectName())  ui->button7->setStyleSheet("background-color:#bc0d15;");
    else if(clickedButtonName==ui->button8->objectName())  ui->button8->setStyleSheet("background-color:#bc0d15;");
    else if(clickedButtonName==ui->button9->objectName())  ui->button9->setStyleSheet("background-color:#bc0d15;");

    if     (buttons.at(index)==ui->button1->objectName())  ui->button1->setStyleSheet("background-color:#0dbc33;");
    else if(buttons.at(index)==ui->button2->objectName())  ui->button2->setStyleSheet("background-color:#0dbc33;");
    else if(buttons.at(index)==ui->button3->objectName())  ui->button3->setStyleSheet("background-color:#0dbc33;");
    else if(buttons.at(index)==ui->button4->objectName())  ui->button4->setStyleSheet("background-color:#0dbc33;");
    else if(buttons.at(index)==ui->button5->objectName())  ui->button5->setStyleSheet("background-color:#0dbc33;");
    else if(buttons.at(index)==ui->button6->objectName())  ui->button6->setStyleSheet("background-color:#0dbc33;");
    else if(buttons.at(index)==ui->button7->objectName())  ui->button7->setStyleSheet("background-color:#0dbc33;");
    else if(buttons.at(index)==ui->button8->objectName())  ui->button8->setStyleSheet("background-color:#0dbc33;");
    else if(buttons.at(index)==ui->button9->objectName())  ui->button9->setStyleSheet("background-color:#0dbc33;");

    QString score=QString("Thats not it, you ended up on %1 round!").arg(roundCounter);
    ui->commentator->setText(score);

    delay(5);

    ui->button1->setStyleSheet("background-color:#bc0d15;");
    ui->button2->setStyleSheet("background-color:#bc0d15;");
    ui->button3->setStyleSheet("background-color:#bc0d15;");
    ui->button4->setStyleSheet("background-color:#bc0d15;");
    ui->button5->setStyleSheet("background-color:#0dbc33;");    //other color for button5
    ui->button6->setStyleSheet("background-color:#bc0d15;");
    ui->button7->setStyleSheet("background-color:#bc0d15;");
    ui->button8->setStyleSheet("background-color:#bc0d15;");
    ui->button9->setStyleSheet("background-color:#bc0d15;");

    playGame=false;
    ui->button5->setText("Restart");
}

void Trainyourmemory::enable_buttons()
{
    ui->button1->setEnabled(true);
    ui->button2->setEnabled(true);
    ui->button3->setEnabled(true);
    ui->button4->setEnabled(true);
    ui->button5->setEnabled(true);
    ui->button6->setEnabled(true);
    ui->button7->setEnabled(true);
    ui->button8->setEnabled(true);
    ui->button9->setEnabled(true);
}
void Trainyourmemory::disable_buttons()
{
    ui->button1->setEnabled(false);
    ui->button2->setEnabled(false);
    ui->button3->setEnabled(false);
    ui->button4->setEnabled(false);
    ui->button5->setEnabled(false);
    ui->button6->setEnabled(false);
    ui->button7->setEnabled(false);
    ui->button8->setEnabled(false);
    ui->button9->setEnabled(false);
}
void Trainyourmemory::delay(int n)
{
        QTime dieTime= QTime::currentTime().addSecs(n);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

Trainyourmemory::~Trainyourmemory()
{
    delete ui;
}

