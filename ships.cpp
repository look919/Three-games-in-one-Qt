#include "ships.h"
#include "ui_ships.h"
#include <QDebug>
#include <QColor>
#include <time.h>
#include <windows.h>
#include <iostream>
#include <QVector>
#include <QMessageBox>
#include <QTime>

using namespace std;

Ships::Ships(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ships)
{
    ui->setupUi(this);

    srand(static_cast<unsigned int>(time(nullptr))); //setting randomness

    //setting the counters;
    playerscore=0;
    cpuscore=0;
    number_of_action=0;
    numberOfShots=0;
    counter=0;
    cpuhelper=0;
    ifhit=false;
    error=false;

    //hide the lvl buttons at the very begining
    ui->easylevel->hide();
    ui->normallevel->hide();
    ui->hardlevel->hide();

    disable_your_buttons();
    disable_cpu_buttons();


    //Setting graphics and hiding textvalue on progressBar
    QPixmap centership(":/img/ship.jpg");
    QPixmap vs(":/img/vs.png");
    ui->vslabel->setPixmap(vs.scaled(25,35));
    ui->graphic->setPixmap(centership.scaled(150,140));
    ui->playerBar->setTextVisible(false);
    ui->cpuBar->setTextVisible(false);
}

void Ships::on_actionButton_clicked()
{
    number_of_action++;

    if(number_of_action==7)
    {
        this->close();
    }
    if(number_of_action==6)
    {
        enable_cpu_buttons();
        disable_shot_buttons();
        ui->actionButton->hide();
        ui->info->setText("You start, goodluck!");
    }
    if(number_of_action==5)
    {
        counter=10;      //Making counter variable useful for CPU shots inteligence
        ui->actionButton->setText("Start the game");
        ui->info->setText("Okay, thats showtime, prepare the cannons!");
    }
    if(number_of_action==4)
    {
        ui->actionButton->setEnabled(false);
        needed=2;
        enable_your_buttons();
        disable_playerships_buttons();
        ui->actionButton->setText("Set ship");
        ui->info->setText("Set your two-level ship on the see on your left, then press button bellow");
    }
    if(number_of_action==3)
    {
        ui->actionButton->setEnabled(false);
        needed=3;
        enable_your_buttons();
        disable_playerships_buttons();
        ui->actionButton->setText("Set ship");
        ui->info->setText("Set your three-level ship on the see on your left, then press button bellow");
    }

    if(number_of_action==2)
    {
        ui->actionButton->setEnabled(false);
        needed=4;
        enable_your_buttons();
        disable_playerships_buttons();
        ui->actionButton->setText("Set ship");
        ui->info->setText("Okay, now set your four-level ship on the see on your left, then press button bellow");
        ui->easylevel->hide();
        ui->normallevel->hide();
        ui->hardlevel->hide();
    }
    if(number_of_action==1)
    {
        ui->actionButton->hide();
        ui->easylevel->show();
        ui->normallevel->show();
        ui->hardlevel->show();
        ui->actionButton->setText("Set Difficulty");
        ui->info->setText("First of all, choose a difficulty level");
        generatecomputerships();
        createcomputerships(4);
        generatecomputerships();
        createcomputerships(3);
        generatecomputerships();
        createcomputerships(2);
    }
}


void Ships::generatecomputerships()
{
    cord1=rand()%6+1;
    cord2=rand()%6+1;

    if(cord1==1) cord1s='A';
    if(cord1==2) cord1s='B';
    if(cord1==3) cord1s='C';
    if(cord1==4) cord1s='D';
    if(cord1==5) cord1s='E';
    if(cord1==6) cord1s='F';

    if(cord2==1) cord2s='1';
    if(cord2==2) cord2s='2';
    if(cord2==3) cord2s='3';
    if(cord2==4) cord2s='4';
    if(cord2==5) cord2s='5';
    if(cord2==6) cord2s='6';

    cord="C_"+cord1s+cord2s;
    savedshipselements.push_back(cord);
}
void Ships::createcomputerships(int n)
{
    if(n==4)
    {
        if(cord2<=3)
        {
            for(int j=0;j<n-1;j++)
            {
                if(cord2s=="1") cord2s="2";
                else if(cord2s=="2") cord2s="3";
                else if(cord2s=="3") cord2s="4";
                else if(cord2s=="4") cord2s="5";
                else if(cord2s=="5") cord2s="6";

                cord="C_"+cord1s+cord2s;
                savedshipselements.push_back(cord);
            }
        }
        else if(cord1<=3)
        {
            for(int j=0;j<n-1;j++)
            {
                if(cord1s=="A") cord1s="B";
                else if(cord1s=="B") cord1s="C";
                else if(cord1s=="C") cord1s="D";
                else if(cord1s=="D") cord1s="E";
                else if(cord1s=="E") cord1s="F";

                cord="C_"+cord1s+cord2s;
                savedshipselements.push_back(cord);
            }
        }
        else if(cord1>3)
        {
            for(int j=0;j<n-1;j++)
            {
                if(cord1s=="F") cord1s="E";
                else if(cord1s=="E") cord1s="D";
                else if(cord1s=="D") cord1s="C";
                else if(cord1s=="C") cord1s="B";
                else if(cord1s=="B") cord1s="A";

                cord="C_"+cord1s+cord2s;
                savedshipselements.push_back(cord);
            }
        }
        for(int i=0;i<computerships.size();i++)
        {
            for(int j=0;j<savedshipselements.size();j++)
            {
                if(savedshipselements.at(j)==computerships.at(i))
                {
                    error=true;
                }
            }
        }
        if(error==true)
        {
            error=false;
            savedshipselements.clear();
            generatecomputerships();
            createcomputerships(n);
        }
        else
        {
            for(int i=0;i<savedshipselements.size();i++)
            {
                computerships.push_back(savedshipselements.at(i));
            }
            savedshipselements.clear();
        }
    }
    else if(n==3)
    {
        if(cord1>3)
        {
            for(int j=0;j<n-1;j++)
            {
                if(cord1s=="F") cord1s="E";
                else if(cord1s=="E") cord1s="D";
                else if(cord1s=="D") cord1s="C";
                else if(cord1s=="C") cord1s="B";
                else if(cord1s=="B") cord1s="A";

                cord="C_"+cord1s+cord2s;
                savedshipselements.push_back(cord);
            }
        }
        else if(cord2>3)
        {
            for(int j=0;j<n-1;j++)
            {
                if(cord2s=="6") cord2s="5";
                else if(cord2s=="5") cord2s="4";
                else if(cord2s=="4") cord2s="3";
                else if(cord2s=="3") cord2s="2";
                else if(cord2s=="2") cord2s="1";

                cord="C_"+cord1s+cord2s;
                savedshipselements.push_back(cord);
            }
        }
        else if(cord2<=3)
        {
            for(int j=0;j<n-1;j++)
            {
                if(cord2s=="1") cord2s="2";
                else if(cord2s=="2") cord2s="3";
                else if(cord2s=="3") cord2s="4";
                else if(cord2s=="4") cord2s="5";

                cord="C_"+cord1s+cord2s;
                savedshipselements.push_back(cord);
            }
        }
        for(int i=0;i<computerships.size();i++)
        {
            for(int j=0;j<savedshipselements.size();j++)
            {
                if(savedshipselements.at(j)==computerships.at(i))
                {
                    error=true;
                }
            }
        }
        if(error==true)
        {
            error=false;
            savedshipselements.clear();
            generatecomputerships();
            createcomputerships(n);
        }
        else
        {
            for(int i=0;i<savedshipselements.size();i++)
            {
                computerships.push_back(savedshipselements.at(i));
            }
            savedshipselements.clear();
        }
    }
    else if(n==2)
    {
        if(cord1<3)
        {
            for(int j=0;j<n-1;j++)
            {
                if(cord1s=="A") cord1s="B";
                else if(cord1s=="B") cord1s="C";
                else if(cord1s=="C") cord1s="D";
                else if(cord1s=="D") cord1s="E";

                cord="C_"+cord1s+cord2s;
                savedshipselements.push_back(cord);
            }
        }
        else if(cord2<=3)
        {
            for(int j=0;j<n-1;j++)
            {
                if(cord2s=="1") cord2s="2";
                else if(cord2s=="2") cord2s="3";
                else if(cord2s=="3") cord2s="4";
                else if(cord2s=="4") cord2s="5";
                else if(cord2s=="5") cord2s="6";

                cord="C_"+cord1s+cord2s;
                savedshipselements.push_back(cord);
            }
        }
        else if(cord2>3)
        {
            for(int j=0;j<n-1;j++)
            {
                if(cord2s=="6") cord2s="5";
                else if(cord2s=="5") cord2s="4";
                else if(cord2s=="4") cord2s="3";
                else if(cord2s=="3") cord2s="2";
                else if(cord2s=="2") cord2s="1";

                cord="C_"+cord1s+cord2s;
                savedshipselements.push_back(cord);
            }
        }
        for(int i=0;i<computerships.size();i++)
        {
            for(int j=0;j<savedshipselements.size();j++)
            {
                if(savedshipselements.at(j)==computerships.at(i))
                {
                    error=true;
                }
            }
        }
        if(error==true)
        {
            error=false;
            savedshipselements.clear();
            generatecomputerships();
            createcomputerships(n);
        }
        else
        {
            for(int i=0;i<savedshipselements.size();i++)
            {
                computerships.push_back(savedshipselements.at(i));
            }
            savedshipselements.clear();
        }
    }
}



void Ships::cpushot()
{
    delay(1);

    cord1=rand()%6+1;
    cord2=rand()%6+1;

    QString alternativeCord1s_bigger;   //hard and medium lvl usage
    QString alternativeCord1s_lower;
    QString alternativeCord2s_bigger;
    QString alternativeCord2s_lower;

    //cord1
    if(cord1==1)
    {
        cord1s='A';
        alternativeCord1s_bigger = 'B';
        alternativeCord1s_lower = 'x';
    }
    if(cord1==2)
    {
        cord1s='B';
        alternativeCord1s_bigger = 'C';
        alternativeCord1s_lower = 'A';
    }
    if(cord1==3)
    {
        cord1s='C';
        alternativeCord1s_bigger = 'D';
        alternativeCord1s_lower = 'B';
    }
    if(cord1==4)
    {
        cord1s='D';
        alternativeCord1s_bigger = 'E';
        alternativeCord1s_lower = 'C';
    }
    if(cord1==5)
    {
        cord1s='E';
        alternativeCord1s_bigger = 'F';
        alternativeCord1s_lower = 'D';
    }
    if(cord1==6)
    {
        cord1s='F';
        alternativeCord1s_bigger = 'x';
        alternativeCord1s_lower = 'E';
    }

   //cord2

    if(cord2==1)
    {
        cord2s='1';
        alternativeCord2s_bigger = '2';
        alternativeCord2s_lower = 'x';
    }
    if(cord2==2)
    {
        cord2s='2';
        alternativeCord2s_bigger = '3';
        alternativeCord2s_lower = '1';
    }
    if(cord2==3)
    {
        cord2s='3';
        alternativeCord2s_bigger = '4';
        alternativeCord2s_lower = '2';
    }
    if(cord2==4)
    {
       cord2s='4';
       alternativeCord2s_bigger = '5';
       alternativeCord2s_lower = '3';
    }
    if(cord2==5)
    {
        cord2s='5';
        alternativeCord2s_bigger = '6';
        alternativeCord2s_lower = '4';
    }
    if(cord2==6)
    {
        cord2s='6';
        alternativeCord2s_bigger = 'x';
        alternativeCord2s_lower = '5';
    }

    cord=cord1s+cord2s;
    QString Cord_bigger1 = alternativeCord1s_bigger+cord2s;
    QString Cord_bigger2 = cord1s+alternativeCord2s_bigger;

    QString Cord_lower1 = alternativeCord1s_lower+cord2s;
    QString Cord_lower2 = cord1s+alternativeCord2s_lower;

    for(int i=0; i<computershots.size();i++)
    {
        if(difficulty==2 && numberOfShots<7)            // if its ahrd computer guesses better within 8 first tries
        {
            if( Cord_bigger1 == computershots.at(i) || Cord_bigger2== computershots.at(i) )
            {
                error=true;
                break;
            }

            if(Cord_lower1 == computershots.at(i) || Cord_lower2 == computershots.at(i) )
            {
                error=true;
                break;
            }
        }
        else if(difficulty==1 && numberOfShots<6)       //if its normal computer guesses better within 6 first tries
        {
            if( Cord_bigger1 == computershots.at(i) || Cord_bigger2== computershots.at(i) )
            {
                error=true;
                break;
            }

            if(Cord_lower1 == computershots.at(i) || Cord_lower2 == computershots.at(i) )
            {
                error=true;
                break;
            }
        }
        else if(difficulty==1 && numberOfShots<4)       //if its easy computer guesses better within 4 first tries
        {
            if( Cord_bigger1 == computershots.at(i) || Cord_bigger2== computershots.at(i) )
            {
                error=true;
                break;
            }

            if(Cord_lower1 == computershots.at(i) || Cord_lower2 == computershots.at(i) )
            {
                error=true;
                break;
            }
        }
        else if(difficulty==2 && numberOfShots<12)      //else if to hard lvl- worse shooting
        {
            if( Cord_bigger1 == computershots.at(i) || Cord_lower2== computershots.at(i) )
            {
                error=true;
                break;
            }
        }
        else if(difficulty==1 && numberOfShots<11)   //else if to normal lvl- worse shooting
        {
            if( Cord_bigger1 == computershots.at(i) || Cord_lower2== computershots.at(i) )
            {
                error=true;
                break;
            }
        }
        else if(difficulty==0 && numberOfShots<10)   //else if to easy lvl- worse shooting
        {
            if( Cord_bigger1 == computershots.at(i) || Cord_lower2== computershots.at(i) )
            {
                error=true;
                break;
            }
        }
        else if(difficulty==2 && numberOfShots<16)      //else if to hard lvl- even worse shooting - final non random
        {
            if( Cord_bigger2 == computershots.at(i))
            {
                error=true;
                break;
            }
        }


        if(cord==computershots.at(i))   //casual checking if random cord is the same like previous shots
        {
            error=true;
            break;
        }
    }

    if(error==true)
    {
        error=false;
        cpushot();
    }
    else
    {
        computershots.push_back(cord);
        numberOfShots++;
        qDebug()<<numberOfShots;
    }


    for(int i=0; i<playerships.size();i++)
    {
        if(cord==playerships.at(i))
        {
            counter=i;
            ifhit=true;
            break;
        }
    }

    if(ifhit==true)
    {
        ifhit=false;
        cpuscore++;
        ui->cpuBar->setValue(cpuscore);
        changecolorifhit();
        cpuwinscreen();

        if(difficulty==0)       //if its easy and computer shot in 4-level
        {
            if(counter<4)
            {
                counter=10;
                for(int i=0;i<4;i++)
                {
                    delay(1);
                    cord=playerships.at(i);
                    computershots.push_back(cord);
                    changecolorifhit();
                    playerships[i]="XX";
                    if(i>0) cpuscore++;
                    ui->cpuBar->setValue(cpuscore);
                }
                ui->cpuBar->setValue(cpuscore);
                 cpuwinscreen();
                 cpushot();
            }
            else if(counter<9)
            {
                counter=10;
                cpushot();
            }
        }
        else if(difficulty==1)  //if its normal and computer shot in 4-level or 3-level ship
        {
            if(counter<4)
            {
                numberOfShots++;
                counter=10;
                for(int i=0;i<4;i++)
                {
                    delay(1);
                    cord=playerships.at(i);
                    computershots.push_back(cord);
                    changecolorifhit();
                    playerships[i]="XX";
                    if(i>0) cpuscore++;
                    ui->cpuBar->setValue(cpuscore);
                }
                ui->cpuBar->setValue(cpuscore);
                 cpuwinscreen();
                 cpushot();
            }
            else if(counter<7)
            {
                numberOfShots++;
                counter=10;
                for(int i=4;i<7;i++)
                {
                     delay(1);
                    cord=playerships.at(i);
                    computershots.push_back(cord);
                    changecolorifhit();
                    playerships[i]="XX";
                    if(i>4) cpuscore++;
                    ui->cpuBar->setValue(cpuscore);
                }
                ui->cpuBar->setValue(cpuscore);
                 cpuwinscreen();
                 cpushot();
            }

            else if(counter<9)
            {
                counter=10;
                cpushot();
            }
        }
        else if(difficulty==2)  //if its hard and computer shot on any ship
        {
            if(counter<4)
            {
                numberOfShots++;
                counter=10;
                for(int i=0;i<4;i++)
                {
                    delay(1);
                    cord=playerships.at(i);
                    computershots.push_back(cord);
                    changecolorifhit();
                    playerships[i]="XX";
                    if(i>0) cpuscore++;
                    ui->cpuBar->setValue(cpuscore);
                }
                ui->cpuBar->setValue(cpuscore);
                 cpuwinscreen();
            }
            else if(counter<7)
            {
                counter=10;
                for(int i=4;i<7;i++)
                {
                     delay(1);
                    cord=playerships.at(i);
                    computershots.push_back(cord);
                    changecolorifhit();
                    playerships[i]="XX";
                    if(i>4) cpuscore++;
                    ui->cpuBar->setValue(cpuscore);
                }
                ui->cpuBar->setValue(cpuscore);
                 cpuwinscreen();
            }
            else if(counter<9)
            {
                counter=10;
                for(int i=7;i<9;i++)
                {
                     delay(1);
                    cord=playerships.at(i);
                    computershots.push_back(cord);
                    changecolorifhit();
                    playerships[i]="XX";
                    if(i>7) cpuscore++;
                    ui->cpuBar->setValue(cpuscore);
                }
                ui->cpuBar->setValue(cpuscore);
                cpuwinscreen();
            }
            cpushot();
        }
    }
    else
    {
        if(cord==ui->A1->objectName()) ui->A1->setStyleSheet("background-color:#0a0a0a");
        else if(cord==ui->B1->objectName()) ui->B1->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->C1->objectName()) ui->C1->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->D1->objectName()) ui->D1->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->E1->objectName()) ui->E1->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->F1->objectName()) ui->F1->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->A2->objectName()) ui->A2->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->B2->objectName()) ui->B2->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->C2->objectName()) ui->C2->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->D2->objectName()) ui->D2->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->E2->objectName()) ui->E2->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->F2->objectName()) ui->F2->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->A3->objectName()) ui->A3->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->B3->objectName()) ui->B3->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->C3->objectName()) ui->C3->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->D3->objectName()) ui->D3->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->E3->objectName()) ui->E3->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->F3->objectName()) ui->F3->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->A4->objectName()) ui->A4->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->B4->objectName()) ui->B4->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->C4->objectName()) ui->C4->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->D4->objectName()) ui->D4->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->E4->objectName()) ui->E4->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->F4->objectName()) ui->F4->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->A5->objectName()) ui->A5->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->B5->objectName()) ui->B5->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->C5->objectName()) ui->C5->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->D5->objectName()) ui->D5->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->E5->objectName()) ui->E5->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->F5->objectName()) ui->F5->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->A6->objectName()) ui->A6->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->B6->objectName()) ui->B6->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->C6->objectName()) ui->C6->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->D6->objectName()) ui->D6->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->E6->objectName()) ui->E6->setStyleSheet("background-color:#0a0a0a;");
        else if(cord==ui->F6->objectName()) ui->F6->setStyleSheet("background-color:#0a0a0a;");
        enable_cpu_buttons();
        disable_shot_buttons();
    }
    ui->info->setStyleSheet("font-size:30px; color:#193c75; font-weight:700;");
    ui->info->setText("Your Turn!");
}

void Ships::changecolorifhit()
{
    if(cord==ui->A1->objectName()) ui->A1->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->B1->objectName()) ui->B1->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->C1->objectName()) ui->C1->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->D1->objectName()) ui->D1->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->E1->objectName()) ui->E1->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->F1->objectName()) ui->F1->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->A2->objectName()) ui->A2->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->B2->objectName()) ui->B2->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->C2->objectName()) ui->C2->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->D2->objectName()) ui->D2->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->E2->objectName()) ui->E2->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->F2->objectName()) ui->F2->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->A3->objectName()) ui->A3->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->B3->objectName()) ui->B3->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->C3->objectName()) ui->C3->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->D3->objectName()) ui->D3->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->E3->objectName()) ui->E3->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->F3->objectName()) ui->F3->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->A4->objectName()) ui->A4->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->B4->objectName()) ui->B4->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->C4->objectName()) ui->C4->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->D4->objectName()) ui->D4->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->E4->objectName()) ui->E4->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->F4->objectName()) ui->F4->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->A5->objectName()) ui->A5->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->B5->objectName()) ui->B5->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->C5->objectName()) ui->C5->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->D5->objectName()) ui->D5->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->E5->objectName()) ui->E5->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->F5->objectName()) ui->F5->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->A6->objectName()) ui->A6->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->B6->objectName()) ui->B6->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->C6->objectName()) ui->C6->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->D6->objectName()) ui->D6->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->E6->objectName()) ui->E6->setStyleSheet("background-color:#dd060d;");
    else if(cord==ui->F6->objectName()) ui->F6->setStyleSheet("background-color:#dd060d;");
}

void Ships::winscreen()
{
    if(playerscore==9)
    {
        QMessageBox::information(this,"You WON","Congratulation, you are the best"                                           " pirate that have ever swam of this ocean",QMessageBox::Ok);
        ui->actionButton->show();
        disable_cpu_buttons();
        ui->info->hide();
        ui->actionButton->setText("Exit");
    }
}

void Ships::cpuwinscreen()
{
    if(cpuscore==9)
    {
        QMessageBox::information(this,"You LOST","Your opponent shot better, another time pirat!",QMessageBox::Ok);
        disable_cpu_buttons();
        ui->actionButton->show();
        ui->info->hide();
        ui->actionButton->setText("Exit");
        delay(30);
    }
}







//SLOTS to choosing ships
void Ships::setshipsbuttonsclicked(QPushButton *p1)
{
    QString cords=p1->objectName();
    playerships.push_back(cords);

    if(needed==4) p1->setStyleSheet("background-color:#80f442; border:1px solid #0b253d;");
    else if(needed==3) p1->setStyleSheet("background-color:#80f442; border:1px solid #d60835;");
    else if(needed==2) p1->setStyleSheet("background-color:#80f442; border:1px solid #0a88ff;");
    counter++;
    if(counter==needed)
    {
        counter=0;
        disable_your_buttons();
        ui->actionButton->setEnabled(true);
    }
    p1->setEnabled(false);
}
void Ships::on_A1_clicked()
{
    setshipsbuttonsclicked(ui->A1);
}

void Ships::on_B1_clicked()
{
   setshipsbuttonsclicked(ui->B1);
}

void Ships::on_C1_clicked()
{
   setshipsbuttonsclicked(ui->C1);
}

void Ships::on_D1_clicked()
{
    setshipsbuttonsclicked(ui->D1);
}
void Ships::on_E1_clicked()
{
    setshipsbuttonsclicked(ui->E1);
}
void Ships::on_F1_clicked()
{
    setshipsbuttonsclicked(ui->F1);
}
void Ships::on_A2_clicked()
{
    setshipsbuttonsclicked(ui->A2);
}
void Ships::on_B2_clicked()
{
    setshipsbuttonsclicked(ui->B2);
}
void Ships::on_C2_clicked()
{
    setshipsbuttonsclicked(ui->C2);
}
void Ships::on_D2_clicked()
{
    setshipsbuttonsclicked(ui->D2);
}
void Ships::on_E2_clicked()
{
   setshipsbuttonsclicked(ui->E2);
}
void Ships::on_F2_clicked()
{
    setshipsbuttonsclicked(ui->F2);
}
void Ships::on_A3_clicked()
{
    setshipsbuttonsclicked(ui->A3);
}
void Ships::on_B3_clicked()
{
    setshipsbuttonsclicked(ui->B3);
}
void Ships::on_C3_clicked()
{
    setshipsbuttonsclicked(ui->C3);
}
void Ships::on_D3_clicked()
{
   setshipsbuttonsclicked(ui->D3);
}
void Ships::on_E3_clicked()
{
   setshipsbuttonsclicked(ui->E3);
}
void Ships::on_F3_clicked()
{
   setshipsbuttonsclicked(ui->F3);
}
void Ships::on_A4_clicked()
{
    setshipsbuttonsclicked(ui->A4);
}
void Ships::on_B4_clicked()
{
   setshipsbuttonsclicked(ui->B4);
}
void Ships::on_C4_clicked()
{
    setshipsbuttonsclicked(ui->C4);
}
void Ships::on_D4_clicked()
{
    setshipsbuttonsclicked(ui->D4);
}
void Ships::on_E4_clicked()
{
    setshipsbuttonsclicked(ui->E4);
}
void Ships::on_F4_clicked()
{
   setshipsbuttonsclicked(ui->F4);
}
void Ships::on_A5_clicked()
{
    setshipsbuttonsclicked(ui->A5);
}
void Ships::on_B5_clicked()
{
    setshipsbuttonsclicked(ui->B5);
}
void Ships::on_C5_clicked()
{
   setshipsbuttonsclicked(ui->C5);
}
void Ships::on_D5_clicked()
{
  setshipsbuttonsclicked(ui->D5);
}
void Ships::on_E5_clicked()
{
   setshipsbuttonsclicked(ui->E5);
}
void Ships::on_F5_clicked()
{
   setshipsbuttonsclicked(ui->F5);
}
void Ships::on_A6_clicked()
{
   setshipsbuttonsclicked(ui->A6);
}
void Ships::on_B6_clicked()
{
    setshipsbuttonsclicked(ui->B6);
}
void Ships::on_C6_clicked()
{
    setshipsbuttonsclicked(ui->C6);
}
void Ships::on_D6_clicked()
{
     setshipsbuttonsclicked(ui->D6);
}
void Ships::on_E6_clicked()
{
     setshipsbuttonsclicked(ui->E6);
}
void Ships::on_F6_clicked()
{
     setshipsbuttonsclicked(ui->F6);
}



//cpubuttons
void Ships::on_computerbutton_clicked(QPushButton *p1)
{
    ui->info->setStyleSheet("");
    QString name=p1->objectName();
     playershots.push_back(name);
    for(int i=0;i<computerships.size();i++)
    {
        if(name==computerships.at(i))
        {
            ifhit=true;
            break;
        }
    }
    if(ifhit==true)
    {
        disable_shot_buttons();
        ifhit=false;
        playerscore++;
        ui->playerBar->setValue(playerscore);
        p1->setStyleSheet("background-color: #26b73c;");
        winscreen();

        int random2=rand()%3+1;
        if(random2==1) ui->info->setText("WE HIT THEM! Lets do it again!");\
        if(random2==2) ui->info->setText("Aye, EAT OUR BULLETS! Give them another one!");
        if(random2==3) ui->info->setText("They HAVE RIPPED! I can smell they fear from this distance, give them next reason to be afraid!");
    }
    else
    {
        p1->setStyleSheet("background-color: #0a0a0a");

        int random2=rand()%3+1;
        if(random2==1) ui->info->setText("Argh, maybe another time!");\
        if(random2==2) ui->info->setText("Damn, lets pray they miss!");
        if(random2==3) ui->info->setText("That miss sounds like trouble!");

        disable_cpu_buttons();
        cpushot();
    }
}



void Ships::on_C_A1_clicked()
{
    on_computerbutton_clicked(ui->C_A1);
}

void Ships::on_C_B1_clicked()
{
    on_computerbutton_clicked(ui->C_B1);
}

void Ships::on_C_C1_clicked()
{
    on_computerbutton_clicked(ui->C_C1);
}

void Ships::on_C_D1_clicked()
{
    on_computerbutton_clicked(ui->C_D1);
}

void Ships::on_C_E1_clicked()
{
    on_computerbutton_clicked(ui->C_E1);
}

void Ships::on_C_F1_clicked()
{
   on_computerbutton_clicked(ui->C_F1);
}
void Ships::on_C_A2_clicked()
{
    on_computerbutton_clicked(ui->C_A2);
}

void Ships::on_C_B2_clicked()
{
  on_computerbutton_clicked(ui->C_B2);
}

void Ships::on_C_C2_clicked()
{
    on_computerbutton_clicked(ui->C_C2);
}

void Ships::on_C_D2_clicked()
{
    on_computerbutton_clicked(ui->C_D2);
}

void Ships::on_C_E2_clicked()
{
    on_computerbutton_clicked(ui->C_E2);
}

void Ships::on_C_F2_clicked()
{
    on_computerbutton_clicked(ui->C_F2);
}
void Ships::on_C_A3_clicked()
{
    on_computerbutton_clicked(ui->C_A3);
}

void Ships::on_C_B3_clicked()
{
    on_computerbutton_clicked(ui->C_B3);
}

void Ships::on_C_C3_clicked()
{
   on_computerbutton_clicked(ui->C_C3);
}

void Ships::on_C_D3_clicked()
{
    on_computerbutton_clicked(ui->C_D3);
}

void Ships::on_C_E3_clicked()
{
    on_computerbutton_clicked(ui->C_E3);
}

void Ships::on_C_F3_clicked()
{
   on_computerbutton_clicked(ui->C_F3);
}
void Ships::on_C_A4_clicked()
{
   on_computerbutton_clicked(ui->C_A4);
}

void Ships::on_C_B4_clicked()
{
    on_computerbutton_clicked(ui->C_B4);
}

void Ships::on_C_C4_clicked()
{
    on_computerbutton_clicked(ui->C_C4);
}

void Ships::on_C_D4_clicked()
{
    on_computerbutton_clicked(ui->C_D4);
}

void Ships::on_C_E4_clicked()
{
    on_computerbutton_clicked(ui->C_E4);
}

void Ships::on_C_F4_clicked()
{
   on_computerbutton_clicked(ui->C_F4);
}
void Ships::on_C_A5_clicked()
{
   on_computerbutton_clicked(ui->C_A5);
}

void Ships::on_C_B5_clicked()
{
    on_computerbutton_clicked(ui->C_B5);
}

void Ships::on_C_C5_clicked()
{
    on_computerbutton_clicked(ui->C_C5);
}

void Ships::on_C_D5_clicked()
{
    on_computerbutton_clicked(ui->C_D5);
}

void Ships::on_C_E5_clicked()
{
    on_computerbutton_clicked(ui->C_E5);
}

void Ships::on_C_F5_clicked()
{
    on_computerbutton_clicked(ui->C_F5);
}
void Ships::on_C_A6_clicked()
{
   on_computerbutton_clicked(ui->C_A6);
}

void Ships::on_C_B6_clicked()
{
    on_computerbutton_clicked(ui->C_B6);
}

void Ships::on_C_C6_clicked()
{
   on_computerbutton_clicked(ui->C_C6);
}

void Ships::on_C_D6_clicked()
{
    on_computerbutton_clicked(ui->C_D6);
}

void Ships::on_C_E6_clicked()
{
   on_computerbutton_clicked(ui->C_E6);
}

void Ships::on_C_F6_clicked()
{
   on_computerbutton_clicked(ui->C_F6);
}


void Ships::disable_your_buttons()
{
    ui->A1->setEnabled(false);
    ui->A2->setEnabled(false);
    ui->A3->setEnabled(false);
    ui->A4->setEnabled(false);
    ui->A5->setEnabled(false);
    ui->A6->setEnabled(false);
    ui->B1->setEnabled(false);
    ui->B2->setEnabled(false);
    ui->B3->setEnabled(false);
    ui->B4->setEnabled(false);
    ui->B5->setEnabled(false);
    ui->B6->setEnabled(false);
    ui->C1->setEnabled(false);
    ui->C2->setEnabled(false);
    ui->C3->setEnabled(false);
    ui->C4->setEnabled(false);
    ui->C5->setEnabled(false);
    ui->C6->setEnabled(false);
    ui->D1->setEnabled(false);
    ui->D2->setEnabled(false);
    ui->D3->setEnabled(false);
    ui->D4->setEnabled(false);
    ui->D5->setEnabled(false);
    ui->D6->setEnabled(false);
    ui->E1->setEnabled(false);
    ui->E2->setEnabled(false);
    ui->E3->setEnabled(false);
    ui->E4->setEnabled(false);
    ui->E5->setEnabled(false);
    ui->E6->setEnabled(false);
    ui->F1->setEnabled(false);
    ui->F2->setEnabled(false);
    ui->F3->setEnabled(false);
    ui->F4->setEnabled(false);
    ui->F5->setEnabled(false);
    ui->F6->setEnabled(false);
}
void Ships::disable_cpu_buttons()
{
    ui->C_A1->setEnabled(false);
    ui->C_A2->setEnabled(false);
    ui->C_A3->setEnabled(false);
    ui->C_A4->setEnabled(false);
    ui->C_A5->setEnabled(false);
    ui->C_A6->setEnabled(false);
    ui->C_B1->setEnabled(false);
    ui->C_B2->setEnabled(false);
    ui->C_B3->setEnabled(false);
    ui->C_B4->setEnabled(false);
    ui->C_B5->setEnabled(false);
    ui->C_B6->setEnabled(false);
    ui->C_C1->setEnabled(false);
    ui->C_C2->setEnabled(false);
    ui->C_C3->setEnabled(false);
    ui->C_C4->setEnabled(false);
    ui->C_C5->setEnabled(false);
    ui->C_C6->setEnabled(false);
    ui->C_D1->setEnabled(false);
    ui->C_D2->setEnabled(false);
    ui->C_D3->setEnabled(false);
    ui->C_D4->setEnabled(false);
    ui->C_D5->setEnabled(false);
    ui->C_D6->setEnabled(false);
    ui->C_E1->setEnabled(false);
    ui->C_E2->setEnabled(false);
    ui->C_E3->setEnabled(false);
    ui->C_E4->setEnabled(false);
    ui->C_E5->setEnabled(false);
    ui->C_E6->setEnabled(false);
    ui->C_F1->setEnabled(false);
    ui->C_F2->setEnabled(false);
    ui->C_F3->setEnabled(false);
    ui->C_F4->setEnabled(false);
    ui->C_F5->setEnabled(false);
    ui->C_F6->setEnabled(false);
}
void Ships::enable_your_buttons()
{
    ui->A1->setEnabled(true);
    ui->A2->setEnabled(true);
    ui->A3->setEnabled(true);
    ui->A4->setEnabled(true);
    ui->A5->setEnabled(true);
    ui->A6->setEnabled(true);
    ui->B1->setEnabled(true);
    ui->B2->setEnabled(true);
    ui->B3->setEnabled(true);
    ui->B4->setEnabled(true);
    ui->B5->setEnabled(true);
    ui->B6->setEnabled(true);
    ui->C1->setEnabled(true);
    ui->C2->setEnabled(true);
    ui->C3->setEnabled(true);
    ui->C4->setEnabled(true);
    ui->C5->setEnabled(true);
    ui->C6->setEnabled(true);
    ui->D1->setEnabled(true);
    ui->D2->setEnabled(true);
    ui->D3->setEnabled(true);
    ui->D4->setEnabled(true);
    ui->D5->setEnabled(true);
    ui->D6->setEnabled(true);
    ui->E1->setEnabled(true);
    ui->E2->setEnabled(true);
    ui->E3->setEnabled(true);
    ui->E4->setEnabled(true);
    ui->E5->setEnabled(true);
    ui->E6->setEnabled(true);
    ui->F1->setEnabled(true);
    ui->F2->setEnabled(true);
    ui->F3->setEnabled(true);
    ui->F4->setEnabled(true);
    ui->F5->setEnabled(true);
    ui->F6->setEnabled(true);
}
void Ships::enable_cpu_buttons()
{
    ui->C_A1->setEnabled(true);
    ui->C_A2->setEnabled(true);
    ui->C_A3->setEnabled(true);
    ui->C_A4->setEnabled(true);
    ui->C_A5->setEnabled(true);
    ui->C_A6->setEnabled(true);
    ui->C_B1->setEnabled(true);
    ui->C_B2->setEnabled(true);
    ui->C_B3->setEnabled(true);
    ui->C_B4->setEnabled(true);
    ui->C_B5->setEnabled(true);
    ui->C_B6->setEnabled(true);
    ui->C_C1->setEnabled(true);
    ui->C_C2->setEnabled(true);
    ui->C_C3->setEnabled(true);
    ui->C_C4->setEnabled(true);
    ui->C_C5->setEnabled(true);
    ui->C_C6->setEnabled(true);
    ui->C_D1->setEnabled(true);
    ui->C_D2->setEnabled(true);
    ui->C_D3->setEnabled(true);
    ui->C_D4->setEnabled(true);
    ui->C_D5->setEnabled(true);
    ui->C_D6->setEnabled(true);
    ui->C_E1->setEnabled(true);
    ui->C_E2->setEnabled(true);
    ui->C_E3->setEnabled(true);
    ui->C_E4->setEnabled(true);
    ui->C_E5->setEnabled(true);
    ui->C_E6->setEnabled(true);
    ui->C_F1->setEnabled(true);
    ui->C_F2->setEnabled(true);
    ui->C_F3->setEnabled(true);
    ui->C_F4->setEnabled(true);
    ui->C_F5->setEnabled(true);
    ui->C_F6->setEnabled(true);
}
void Ships::disable_shot_buttons()
{
    for(int i=0;i<playershots.size();i++)
    {
        if(playershots.at(i)==ui->C_A1->objectName()) ui->C_A1->setEnabled(false);
        if(playershots.at(i)==ui->C_B1->objectName()) ui->C_B1->setEnabled(false);
        if(playershots.at(i)==ui->C_C1->objectName()) ui->C_C1->setEnabled(false);
        if(playershots.at(i)==ui->C_D1->objectName()) ui->C_D1->setEnabled(false);
        if(playershots.at(i)==ui->C_E1->objectName()) ui->C_E1->setEnabled(false);
        if(playershots.at(i)==ui->C_F1->objectName()) ui->C_F1->setEnabled(false);
        if(playershots.at(i)==ui->C_A2->objectName()) ui->C_A2->setEnabled(false);
        if(playershots.at(i)==ui->C_B2->objectName()) ui->C_B2->setEnabled(false);
        if(playershots.at(i)==ui->C_C2->objectName()) ui->C_C2->setEnabled(false);
        if(playershots.at(i)==ui->C_D2->objectName()) ui->C_D2->setEnabled(false);
        if(playershots.at(i)==ui->C_E2->objectName()) ui->C_E2->setEnabled(false);
        if(playershots.at(i)==ui->C_F2->objectName()) ui->C_F2->setEnabled(false);
        if(playershots.at(i)==ui->C_A3->objectName()) ui->C_A3->setEnabled(false);
        if(playershots.at(i)==ui->C_B3->objectName()) ui->C_B3->setEnabled(false);
        if(playershots.at(i)==ui->C_C3->objectName()) ui->C_C3->setEnabled(false);
        if(playershots.at(i)==ui->C_D3->objectName()) ui->C_D3->setEnabled(false);
        if(playershots.at(i)==ui->C_E3->objectName()) ui->C_E3->setEnabled(false);
        if(playershots.at(i)==ui->C_F3->objectName()) ui->C_F3->setEnabled(false);
        if(playershots.at(i)==ui->C_A4->objectName()) ui->C_A4->setEnabled(false);
        if(playershots.at(i)==ui->C_B4->objectName()) ui->C_B4->setEnabled(false);
        if(playershots.at(i)==ui->C_C4->objectName()) ui->C_C4->setEnabled(false);
        if(playershots.at(i)==ui->C_D4->objectName()) ui->C_D4->setEnabled(false);
        if(playershots.at(i)==ui->C_E4->objectName()) ui->C_E4->setEnabled(false);
        if(playershots.at(i)==ui->C_F4->objectName()) ui->C_F4->setEnabled(false);
        if(playershots.at(i)==ui->C_A5->objectName()) ui->C_A5->setEnabled(false);
        if(playershots.at(i)==ui->C_B5->objectName()) ui->C_B5->setEnabled(false);
        if(playershots.at(i)==ui->C_C5->objectName()) ui->C_C5->setEnabled(false);
        if(playershots.at(i)==ui->C_D5->objectName()) ui->C_D5->setEnabled(false);
        if(playershots.at(i)==ui->C_E5->objectName()) ui->C_E5->setEnabled(false);
        if(playershots.at(i)==ui->C_F5->objectName()) ui->C_F5->setEnabled(false);
        if(playershots.at(i)==ui->C_A6->objectName()) ui->C_A6->setEnabled(false);
        if(playershots.at(i)==ui->C_B6->objectName()) ui->C_B6->setEnabled(false);
        if(playershots.at(i)==ui->C_C6->objectName()) ui->C_C6->setEnabled(false);
        if(playershots.at(i)==ui->C_D6->objectName()) ui->C_D6->setEnabled(false);
        if(playershots.at(i)==ui->C_E6->objectName()) ui->C_E6->setEnabled(false);
        if(playershots.at(i)==ui->C_F6->objectName()) ui->C_F6->setEnabled(false);
    }
}
void Ships::disable_playerships_buttons()
{
    for(int i=0;i<playerships.size();i++)
    {
        if(playerships.at(i)==ui->A1->objectName()) ui->A1->setEnabled(false);
        if(playerships.at(i)==ui->B1->objectName()) ui->B1->setEnabled(false);
        if(playerships.at(i)==ui->C1->objectName()) ui->C1->setEnabled(false);
        if(playerships.at(i)==ui->D1->objectName()) ui->D1->setEnabled(false);
        if(playerships.at(i)==ui->E1->objectName()) ui->E1->setEnabled(false);
        if(playerships.at(i)==ui->F1->objectName()) ui->F1->setEnabled(false);
        if(playerships.at(i)==ui->A2->objectName()) ui->A2->setEnabled(false);
        if(playerships.at(i)==ui->B2->objectName()) ui->B2->setEnabled(false);
        if(playerships.at(i)==ui->C2->objectName()) ui->C2->setEnabled(false);
        if(playerships.at(i)==ui->D2->objectName()) ui->D2->setEnabled(false);
        if(playerships.at(i)==ui->E2->objectName()) ui->E2->setEnabled(false);
        if(playerships.at(i)==ui->F2->objectName()) ui->F2->setEnabled(false);
        if(playerships.at(i)==ui->A3->objectName()) ui->A3->setEnabled(false);
        if(playerships.at(i)==ui->B3->objectName()) ui->B3->setEnabled(false);
        if(playerships.at(i)==ui->C3->objectName()) ui->C3->setEnabled(false);
        if(playerships.at(i)==ui->D3->objectName()) ui->D3->setEnabled(false);
        if(playerships.at(i)==ui->E3->objectName()) ui->E3->setEnabled(false);
        if(playerships.at(i)==ui->F3->objectName()) ui->F3->setEnabled(false);
        if(playerships.at(i)==ui->A4->objectName()) ui->A4->setEnabled(false);
        if(playerships.at(i)==ui->B4->objectName()) ui->B4->setEnabled(false);
        if(playerships.at(i)==ui->C4->objectName()) ui->C4->setEnabled(false);
        if(playerships.at(i)==ui->D4->objectName()) ui->D4->setEnabled(false);
        if(playerships.at(i)==ui->E4->objectName()) ui->E4->setEnabled(false);
        if(playerships.at(i)==ui->F4->objectName()) ui->F4->setEnabled(false);
        if(playerships.at(i)==ui->A5->objectName()) ui->A5->setEnabled(false);
        if(playerships.at(i)==ui->B5->objectName()) ui->B5->setEnabled(false);
        if(playerships.at(i)==ui->C5->objectName()) ui->C5->setEnabled(false);
        if(playerships.at(i)==ui->D5->objectName()) ui->D5->setEnabled(false);
        if(playerships.at(i)==ui->E5->objectName()) ui->E5->setEnabled(false);
        if(playerships.at(i)==ui->F5->objectName()) ui->F5->setEnabled(false);
        if(playerships.at(i)==ui->A6->objectName()) ui->A6->setEnabled(false);
        if(playerships.at(i)==ui->B6->objectName()) ui->B6->setEnabled(false);
        if(playerships.at(i)==ui->C6->objectName()) ui->C6->setEnabled(false);
        if(playerships.at(i)==ui->D6->objectName()) ui->D6->setEnabled(false);
        if(playerships.at(i)==ui->E6->objectName()) ui->E6->setEnabled(false);
        if(playerships.at(i)==ui->F6->objectName()) ui->F6->setEnabled(false);
    }
}
void Ships::delay(int n)
{
    QTime clock= QTime::currentTime().addSecs(n);
    while (QTime::currentTime() < clock)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void Ships::on_easylevel_clicked()
{
    difficulty=0;
    ui->actionButton->show();
    ui->info->setText("Easy? If thats all you can handle landlubber!");
}
void Ships::on_normallevel_clicked()
{
    difficulty=1;
    ui->actionButton->show();
    ui->info->setText("Normal? Lets see how you shoot pirat!");
}
void Ships::on_hardlevel_clicked()
{
    difficulty=2;
    ui->actionButton->show();
    ui->info->setText("Hard? You had my attention, now you got my respect captain!");
}

Ships::~Ships()
{
    delete ui;
}
