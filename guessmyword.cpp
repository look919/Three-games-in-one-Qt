#include "guessmyword.h"
#include "ui_guessmyword.h"
#include<windows.h>
#include<QMessageBox>
#include <QDebug>
#include <QTime>

Guessmyword::Guessmyword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Guessmyword)
{
    ui->setupUi(this);
    ui->letter->setMaxLength(1);

    earLetters.push_back("e");  //3 most common letters in english dictionary, just to hard level usage;
    earLetters.push_back("a");
    earLetters.push_back("r");

    ui->startquitbutton->setEnabled(false);
    ui->table->setText("Choose difficulty:");
    ui->table->setAlignment(Qt::AlignCenter);
    ui->easylevel->setStyleSheet("background-color:#4a39dd;");
    ui->normallevel->setStyleSheet("background-color:#4a39dd;");
    ui->hardlevel->setStyleSheet("background-color:#4a39dd;");
    ui->giveupbutton->hide();
    ui->lettershower->hide();
    ui->commentator->hide();
    ui->guesstheletterbutton->hide();
    ui->guessthepasswordbutton->hide();
    ui->letter->hide();
    ui->password->hide();
    ui->remainingletterslabel->hide();
    gamecounter=0;
    lettersleft=10;
}

void Guessmyword::on_startquitbutton_clicked()
{
  addwordstolist();

    if(gamecounter==0)
    {
       ui->table->setStyleSheet("font-size: 40px; color:black; border: 1px solid blue; background-color: #42f4c8;");
       ui->easylevel->hide();
       ui->normallevel->hide();
       ui->hardlevel->hide();
       ui->commentator->show();
       ui->commentator->setText("Try your luck!");
       ui->startquitbutton->hide();
       ui->guesstheletterbutton->show();
       ui->guessthepasswordbutton->show();
       ui->letter->show();
       ui->password->show();
       ui->remainingletterslabel->show();

       setui();
    }
    else if(gamecounter==1)
    {
        ui->commentator->show();
        ui->commentator->setText("I am sure i can beat you!");
        ui->startquitbutton->hide();
        ui->guessthepasswordbutton->show();
        ui->guessthepasswordbutton->setText("Enter your word!");
        ui->password->show();
    }
}

void Guessmyword::on_guesstheletterbutton_clicked()
{
    if(!ui->letter->text().isEmpty())
    {
        ui->letter->text().toLower();
        generatedletters.push_back(ui->letter->text());
        lettersleft--;
        if(lettersleft==0)
        {
            ui->giveupbutton->show();
            ui->letter->setEnabled(false);
            ui->guesstheletterbutton->setEnabled(false);
        }

        setui();
    }
}

void Guessmyword::on_guessthepasswordbutton_clicked()
{
    if(gamecounter==0)
    {
        if(!ui->password->text().isEmpty())
        {
            gamecounter++;
            ui->password->text().toLower();
            if(ui->password->text()==generatedword)
            {
                playerscore=10-lettersleft;
                computerscore=100;
                QMessageBox::information(this,"Right answer","Bravo! You found the right word!");
                ui->table->setText(generatedword);
                ui->table->setAlignment(Qt::AlignCenter);
            }
            else
            {
                computerscore=100;
                playerscore=0;
                QMessageBox::information(this,"Wrong answer","Unfortunately, your answer is wrong");
                ui->table->setText(generatedword);
                ui->table->setAlignment(Qt::AlignCenter);
            }
            ui->giveupbutton->hide();
            ui->commentator->hide();
            ui->guesstheletterbutton->hide();
            ui->guessthepasswordbutton->hide();
            ui->letter->hide();
            ui->password->hide();
            ui->startquitbutton->show();
            ui->startquitbutton->setText("Continue");
            ui->password->clear();
            ui->commentator->setText("Was it that hard?");
        }
   }
   else if(gamecounter==1)
   {
       if((ui->password->text().length()>=5) && (ui->password->text().length()<=12))
       {
           ui->password->text().toLower();
           lettersleft=10;
           generatedword=ui->password->text();
           wordlength=ui->password->text().length();
           ui->commentator->setText("I am sure i can beat you!");
           ui->guessthepasswordbutton->hide();
           ui->password->setEnabled(false);
           computerguesses();
       }
       else
       {
           ui->commentator->setText("Your word should contain from 5 to 12 letters!");
           ui->commentator->show();
       }
   }
    else if(gamecounter==2)
    {
        this->close();
    }
}

void Guessmyword::createtableword()
{
    tableword.clear();
    ui->letter->clear();
    letterexist=false;

    for(int i=0;i<wordlength;i++)
    {
        ifexist=false;


        for (int j=0;j<generatedletters.size();j++)
        {
            if(level==1 && i==0 && gamecounter==0)    //statment for easy level - free first letter
            {
                tableword+=generatedword[i]+" ";
                ifexist=true;
                break;
            }
            else if(level==2 && i==wordlength-1 && gamecounter==0)    //statement for normal level - free last letter
            {
                tableword+=generatedword[i]+" ";
                ifexist=true;
                break;
            }
            if(generatedletters[j]==generatedword[i])   //casual statment
            {
                tableword+=generatedword[i]+" ";
                ifexist=true;
                break;
            }
        }
         if(ifexist==false) tableword+="_ ";


    }
    if(generatedletters.size()>=1)
    {
        int lastIndex=generatedletters.size()-1;
        for(int k=0;k<wordlength;k++)
        {
            if(generatedletters[lastIndex]==generatedword[k])
            {
                letterexist=true;
                break;
            }
        }
    }
}

void Guessmyword::setui()
{
    createtableword();
    ui->table->setText(tableword);
    ui->table->setAlignment(Qt::AlignCenter);

    QString s=QString::number(lettersleft); //converts the number of letters left to string
    QString text=QString("Remaining letters: %1").arg(s);
    ui->remainingletterslabel->setText(text);
}


void Guessmyword::computerguesses()
{
    //independent elements no matter which level we play on
    wordlisthaslessthantenelements=false;

    generatedletters.clear();
    generatedletters.push_back("Ó");
    ui->commentator->setText("There we go!");

    for(int i=0;i<listofwords.size();i++)      //Deleting words that are longer or shorter
    {
        if(listofwords[i].length()!=wordlength)
        {
            listofwords.erase(listofwords.begin()+i);
            i=-1;
        }
    }

    if(level==3)
     {
         betteralphabetinvec();
         alphabet.pop_back();   //Eliminaiting "e","a" and "r" from alphabet, because its used separately
         alphabet.pop_back();
         alphabet.pop_back();

         int k=3;

         while(k>0 && computerscore==100)
         {
             int random=rand()%k;
             delay(3);
             setuiforpc(earLetters[random]);
             makewordlistshorter(earLetters[random]);
             conditionaloflistelements();
             debugger();
             earLetters.erase(earLetters.begin()+random);
             k--;
         }
            k=0;
             while(computerscore==100)
             {
                 if(!wordlisthaslessthantenelements)
                 {
                     delay(3);
                     randomchar();
                     setuiforpc(randchar);
                     makewordlistshorter(randchar);
                     conditionaloflistelements();
                     debugger();
                 }
                 else
                 {
                     delay(3);
                     for(int j=0;j<guessedletters.size();j++)
                     {
                         if(listofwords.at(0)[k] == guessedletters[j])
                            {
                                 j=0;
                                 k++;
                                 if(k==listofwords[0].size()) k=0;
                            }

                     }
                     randchar=listofwords[0][k];
                     setuiforpc(randchar);
                     makewordlistshorter(randchar);
                     conditionaloflistelements();
                     debugger();
                     k++;
                 }
            }

             endscreen();
    }                       //end of lvl hard starting normal bellow
    else if(level==2)
    {
        betteralphabetinvec();
        alphabet.pop_back();

        delay(3);
        setuiforpc("e");
        makewordlistshorter("e");
        conditionaloflistelements();
        debugger();


        int k=3;

        while(k>0 && computerscore==100)
        {
            int random=rand()%alphabet.size();
            delay(3);
            setuiforpc(alphabet[random]);
            makewordlistshorter(alphabet[random]);
            conditionaloflistelements();
            debugger();
            alphabet.erase(alphabet.begin()+random);
            k--;
        }
        worsealphabetinvec();
        k=0;

        while(computerscore==100)
         {
             if(!wordlisthaslessthantenelements)
             {
                 delay(3);
                 randomchar();
                 setuiforpc(randchar);
                 makewordlistshorter(randchar);
                 conditionaloflistelements();
                 debugger();
             }
             else
             {
                 delay(3);
                 for(int j=0;j<guessedletters.size();j++)
                 {
                     if(listofwords.at(0)[k] == guessedletters[j])
                        {
                             j=0;
                             k++;
                             if(k==listofwords[0].size()) k=0;
                        }
                 }
                 randchar=listofwords[0][k];
                 setuiforpc(randchar);
                 makewordlistshorter(randchar);
                 conditionaloflistelements();
                 debugger();
                 k++;
             }
        }
         endscreen();
    }           // End of normal, beginning of easy
    else
    {
        betteralphabetinvec();
        worsealphabetinvec();

        int k=0;
        while(computerscore==100)
         {
             if(!wordlisthaslessthantenelements)
             {
                 delay(3);
                 randomchar();
                 setuiforpc(randchar);
                 makewordlistshorter(randchar);
                 conditionaloflistelements();
                 debugger();
             }
             else
             {
                 delay(3);
                 for(int j=0;j<guessedletters.size();j++)
                 {
                     if(listofwords.at(0)[k] == guessedletters[j])
                        {
                             j = -1;
                             k++;
                             if(k==listofwords[0].size()) break;
                        }

                 }
                 randchar=listofwords[0][k];
                 setuiforpc(randchar);
                 makewordlistshorter(randchar);
                 conditionaloflistelements();
                 debugger();
                 k++;
             }
        }
         endscreen();
    }
}




void Guessmyword::setuiforpc(QString s1)
{
    QPixmap sad(":/img/sad.jpg");
    QPixmap happy(":/img/happy.jpg");
    ui->lettershower->setText(s1);
    ui->lettershower->show();

    generatedletters.push_back(s1);
    createtableword();
    ui->table->setText(tableword);
    ui->table->setAlignment(Qt::AlignCenter);

    lettersleft--;
    QString s=QString::number(lettersleft);
    QString text=QString("Remaining letters: %1").arg(s);
    ui->remainingletterslabel->setText(text);

    if(letterexist)
    {
        ui->commentator->setPixmap(happy.scaled(60,60));
        letterexist=false;
    }
    else
    {
        ui->commentator->setPixmap(sad.scaled(60,60));
    }
}

void Guessmyword::makewordlistshorter(QString s1)
{
    letterexist=false;
    for(int i=0;i<generatedword.size();i++)          //Checking the position --idk why it didnt work in createtable function
    {
        if(generatedword[i]==s1)
        {
            letterposition=i;
            letterexist=true;
        }
    }

    if(letterexist)
    {
        for(int i=0;i<listofwords.size();i++)
        {
           if(listofwords.at(i)[letterposition]!=s1)
           {
               listofwords.erase(listofwords.begin()+i);
               i=-1;
           }
        }
    }
    else
    {
        for(int i=0;i<listofwords.size();i++)
        {
            for(int j=0;j<listofwords[i].length();j++)
               {
                    if(listofwords[i].at(j)==s1)
                    {
                        listofwords.erase(listofwords.begin()+i);
                        i=-1;
                        break;
                    }
               }
        }
    }
}

void Guessmyword::conditionaloflistelements()
{
    if(listofwords.size()==0)
    {
        ui->lettershower->setStyleSheet("font-size: 14px;");
        ui->lettershower->setText("I Give up!");
        ui->lettershower->setStyleSheet("font-size: 24px;");
        computerscore=0;
        delay(5);
        ui->table->setText(generatedword);
        ui->table->setAlignment(Qt::AlignCenter);
        ui->commentator->setText("That was way too hard for me!");
        ui->lettershower->hide();
        ui->password->hide();
        ifexist=true;
    }
    else if (listofwords.size()<=2)
    {
        ui->lettershower->setStyleSheet("font-size: 16px;");
        ui->lettershower->setText("In my opinion the world is ->");
        ui->commentator->setStyleSheet("font-size: 24px;");
        ui->commentator->setText(listofwords[0]);
        delay(5);
        if(listofwords[0]==generatedword)
        {
            computerscore=10-lettersleft;
            ui->commentator->setStyleSheet("font-size: 14px;");
            ui->commentator->setText("Yay! I was right!");
            ui->lettershower->hide();
            ui->password->hide();
        }
        else
        {
            computerscore=0;
            ui->commentator->setStyleSheet("font-size: 14px;");
            ui->commentator->setText("Oh no! How could i forget about that word!");
            ui->lettershower->hide();
            ui->password->hide();
        }
        ui->table->setText(generatedword);
        ui->table->setAlignment(Qt::AlignCenter);
    }
    else if(lettersleft==0)
    {
        srand((static_cast<unsigned int>(time(nullptr))));

        int random=rand()%listofwords.size();
        ui->lettershower->setStyleSheet("font-size: 14px;");
        ui->lettershower->setText("In my opinion the world is ->");
        ui->commentator->setStyleSheet("font-size: 24px;");
        ui->commentator->setText(listofwords[random]);
        delay(5);
        if(listofwords[random]==generatedword)
        {
            computerscore=10-lettersleft;
            ui->commentator->setStyleSheet("font-size: 14px;");
            ui->commentator->setText("Yay! I was right!");
            ui->lettershower->hide();
             ui->password->hide();
        }
        else
        {
            computerscore=0;
            ui->commentator->setStyleSheet("font-size: 14px;");
            ui->commentator->setText("Oh no! I should've know that!");
            ui->lettershower->hide();
             ui->password->hide();
        }
        ui->table->setText(generatedword);
        ui->table->setAlignment(Qt::AlignCenter);
    }
    else if(level==3 && listofwords.size()<30)
    {
        wordlisthaslessthantenelements=true;
    }
    else if(level==2 && listofwords.size()<20)
    {
        wordlisthaslessthantenelements=true;
    }
    else if(level==1 && listofwords.size()<14)
    {
        wordlisthaslessthantenelements=true;
    }
}

void Guessmyword::endscreen()
{
    delay(3);
    ui->commentator->setText("Lets find out who was better");
    delay(4);

    ui->lettershower->setStyleSheet("font-size:11px");
    ui->lettershower->show();

    if(computerscore>0 && playerscore>0)
    {
            ui->lettershower->setText("Number of rounds needed to find out the right word ->");

            QString playertext;
            QString cputext;
            QString computerresult=QString::number(computerscore);
            QString playerresult=QString::number(playerscore);

            if(computerscore==10) cputext = "Computer: 10";
            else cputext=QString("Computer: %1").arg(computerresult);

            if(playerscore==10)  playertext = "Player: 10";                 //Didnt work for 10th round guess
            else playertext=QString("Player: %1").arg(playerresult);

            ui->commentator->setText(cputext+"\n"+playertext);

            delay(7);
            if(computerscore<playerscore)
            {
              ui->commentator->setText("I was better this time, well fought anyway");
              ui->lettershower->hide();
              ui->guessthepasswordbutton->show();
              ui->guessthepasswordbutton->setText("Quit");
              gamecounter++;
            }
            else if(playerscore<computerscore)
            {
                ui->commentator->setText("Wow, you gave me no chance, congratulation!");
                ui->lettershower->hide();
                ui->guessthepasswordbutton->show();
                ui->guessthepasswordbutton->setText("Quit");
                gamecounter++;
            }
            else
            {
                ui->commentator->setText("Draw? I can accept that, how about rematch?");
                ui->lettershower->hide();
                ui->guessthepasswordbutton->show();
                ui->guessthepasswordbutton->setText("Quit");
                gamecounter++;
            }
    }
    else if(computerscore>0 && playerscore==0)
    {
        ui->lettershower->setText("Number of rounds needed to find out the right word ->");
        QString computerresult=QString::number(computerscore);
        QString cputext=QString("Computer: %1").arg(computerresult);
        QString playertext=QString("Player: Didn't guess");
        ui->commentator->setText(cputext+"\n"+playertext);

        delay(7);
          ui->commentator->setText("Way too easy man!");
          ui->lettershower->hide();
          ui->guessthepasswordbutton->show();
          ui->guessthepasswordbutton->setText("Quit");
          gamecounter++;
    }
    else if(computerscore==0 && playerscore>0)
    {
        ui->lettershower->setText("Number of rounds needed to find out the right word ->");
        QString computerresult=QString::number(computerscore);
        QString playertext=QString("Player: %1").arg(computerresult);
        QString cputext=QString("Computer: Didn't guess");
        ui->commentator->setText(cputext+"\n"+playertext);

        delay(7);
          ui->commentator->setText("Ugh, I got smashed, another time clever boy!");
          ui->lettershower->hide();
          ui->guessthepasswordbutton->show();
          ui->guessthepasswordbutton->setText("Quit");
          gamecounter++;
    }
    else if(computerscore==0 && playerscore==0)
    {
        ui->lettershower->setText("Number of rounds needed to find out the right word ->");
        QString computerresult=QString::number(computerscore);
        QString cputext=QString("Computer: Didn't guess");
        QString playertext=QString("Player: Didn't guess");
        ui->commentator->setText(cputext+"\n"+playertext);

        delay(7);
          ui->commentator->setText("Cmon, we are both so bad! Let it be our secret!");
          ui->lettershower->hide();
          ui->guessthepasswordbutton->show();
          ui->guessthepasswordbutton->setText("Quit");
          gamecounter++;
    }
}

void Guessmyword::delay(int n)
{
        QTime dieTime= QTime::currentTime().addSecs(n);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Guessmyword::betteralphabetinvec()
{

    alphabet.push_back("i");
    alphabet.push_back("o");
    alphabet.push_back("t");
    alphabet.push_back("n");
    alphabet.push_back("s");
    alphabet.push_back("l");
    alphabet.push_back("c");
    alphabet.push_back("u");
    alphabet.push_back("d");
    alphabet.push_back("p");
    alphabet.push_back("m");
    alphabet.push_back("h");

    alphabet.push_back("a");
    alphabet.push_back("r");
    alphabet.push_back("e");  //to pop out those letters on the hard level
}

void Guessmyword::worsealphabetinvec()
{
    alphabet.push_back("g");
    alphabet.push_back("b");
    alphabet.push_back("f");
    alphabet.push_back("y");
    alphabet.push_back("w");
    alphabet.push_back("k");
    alphabet.push_back("v");
    alphabet.push_back("x");
    alphabet.push_back("z");
    alphabet.push_back("j");
    alphabet.push_back("q");
}

void Guessmyword::randomchar()
{
    int random;
    bool error=false;

    random=rand()%alphabet.size();
    randchar=alphabet[random];

    for(int i=0;i<guessedletters.size();i++)
    {
        if(guessedletters[i]==randchar) error=true;
    }
    if(error) randomchar();
    else guessedletters.push_back(randchar);
}

void Guessmyword::on_giveupbutton_clicked()
{
     gamecounter++;

     computerscore=100;
     playerscore=0;
     QMessageBox::information(this,"France","Was it that hard?");
     ui->table->setText(generatedword);
     ui->table->setAlignment(Qt::AlignCenter);

     ui->giveupbutton->hide();
     ui->commentator->hide();
     ui->guesstheletterbutton->hide();
     ui->guessthepasswordbutton->hide();
     ui->letter->hide();
     ui->password->hide();
     ui->startquitbutton->show();
     ui->startquitbutton->setText("Continue");
     ui->password->clear();
     ui->commentator->setText("Was it that hard?");
}

void Guessmyword::on_easylevel_clicked()
{
     ui->startquitbutton->setEnabled(true);
    ui->easylevel->setStyleSheet("background-color:#12aa75;");
    ui->normallevel->setStyleSheet("background-color:#4a39dd;");
    ui->hardlevel->setStyleSheet("background-color:#4a39dd;");
    level=1;

    ui->table->setStyleSheet("font-size: 13px; color:black; border: 1px solid blue; background-color: #42f4c8;");
    ui->table->setText("Word length 7-12 letters\n"
                       "First letter is given for free after first shot\n"
                       "Computer guessing fully random");
}

void Guessmyword::on_normallevel_clicked()
{
     ui->startquitbutton->setEnabled(true);
    ui->easylevel->setStyleSheet("background-color:#4a39dd;");
    ui->normallevel->setStyleSheet("background-color:#12aa75;");
    ui->hardlevel->setStyleSheet("background-color:#4a39dd;");
    level=2;

    ui->table->setStyleSheet("font-size: 13px; color:black; border: 1px solid blue; background-color: #42f4c8;");
    ui->table->setText("Word length 6-12 letters\n"
                       "Last letter is given for free after first shot\n"
                       "Computer guessing partially random");
}

void Guessmyword::on_hardlevel_clicked()
{

    ui->startquitbutton->setEnabled(true);
    level=3;
    ui->easylevel->setStyleSheet("background-color:#4a39dd;");
    ui->normallevel->setStyleSheet("background-color:#4a39dd;");
    ui->hardlevel->setStyleSheet("background-color:#12aa75;");

    ui->table->setStyleSheet("font-size: 13px; color:black; border: 1px solid blue; background-color: #42f4c8;");
    ui->table->setText("Word length 5-12 letters\n"
                       "Without free letters\n"
                       "Computer guessing in the minority random");
}

void Guessmyword::debugger()
{

    qDebug()<<listofwords.size()/2;

}

void Guessmyword::addwordstolist()
{
    srand((static_cast<unsigned int>(time(nullptr))));

    listofwords.push_back( "abandon" );         //should be 1312 words that have length from 5 to 12 characters
    listofwords.push_back( "ability" );
    listofwords.push_back( "abortion" );
    listofwords.push_back( "absence" );
    listofwords.push_back( "absolute" );
    listofwords.push_back( "absolutely" );
    listofwords.push_back( "academic" );
    listofwords.push_back( "accident" );
    listofwords.push_back( "accompany" );
    listofwords.push_back( "accomplish" );
    listofwords.push_back( "according" );
    listofwords.push_back( "account" );
    listofwords.push_back( "accurate" );
    listofwords.push_back( "achieve" );
    listofwords.push_back( "achievement" );
    listofwords.push_back( "acknowledge" );
    listofwords.push_back( "acquire" );
    listofwords.push_back( "activist" );
    listofwords.push_back( "activity" );
    listofwords.push_back( "actress" );
    listofwords.push_back( "actually" );
    listofwords.push_back( "addition" );
    listofwords.push_back( "additional" );
    listofwords.push_back( "address" );
    listofwords.push_back( "adequate" );
    listofwords.push_back( "adjustment" );
    listofwords.push_back( "administration" );
    listofwords.push_back( "administrator" );
    listofwords.push_back( "admission" );
    listofwords.push_back( "adolescent" );
    listofwords.push_back( "advance" );
    listofwords.push_back( "advanced" );
    listofwords.push_back( "advantage" );
    listofwords.push_back( "adventure" );
    listofwords.push_back( "advertising" );
    listofwords.push_back( "adviser" );
    listofwords.push_back( "advocate" );
    listofwords.push_back( "african" );
    listofwords.push_back( "afternoon" );
    listofwords.push_back( "against" );
    listofwords.push_back( "aggressive" );
    listofwords.push_back( "agreement" );
    listofwords.push_back( "agricultural" );
    listofwords.push_back( "aircraft" );
    listofwords.push_back( "airline" );
    listofwords.push_back( "airport" );
    listofwords.push_back( "alcohol" );
    listofwords.push_back( "alliance" );
    listofwords.push_back( "already" );
    listofwords.push_back( "alternative" );
    listofwords.push_back( "although" );
    listofwords.push_back( "amazing" );
    listofwords.push_back( "american" );
    listofwords.push_back( "analysis" );
    listofwords.push_back( "analyst" );
    listofwords.push_back( "analyze" );
    listofwords.push_back( "ancient" );
    listofwords.push_back( "anniversary" );
    listofwords.push_back( "announce" );
    listofwords.push_back( "another" );
    listofwords.push_back( "anticipate" );
    listofwords.push_back( "anxiety" );
    listofwords.push_back( "anybody" );
    listofwords.push_back( "anymore" );
    listofwords.push_back( "anything" );
    listofwords.push_back( "anywhere" );
    listofwords.push_back( "apartment" );
    listofwords.push_back( "apparent" );
    listofwords.push_back( "apparently" );
    listofwords.push_back( "appearance" );
    listofwords.push_back( "application" );
    listofwords.push_back( "appoint" );
    listofwords.push_back( "appointment" );
    listofwords.push_back( "appreciate" );
    listofwords.push_back( "approach" );
    listofwords.push_back( "appropriate" );
    listofwords.push_back( "approval" );
    listofwords.push_back( "approve" );
    listofwords.push_back( "approximately" );
    listofwords.push_back( "architect" );
    listofwords.push_back( "argument" );
    listofwords.push_back( "arrange" );
    listofwords.push_back( "arrangement" );
    listofwords.push_back( "arrival" );
    listofwords.push_back( "article" );
    listofwords.push_back( "artistic" );
    listofwords.push_back( "assault" );
    listofwords.push_back( "assessment" );
    listofwords.push_back( "assignment" );
    listofwords.push_back( "assistance" );
    listofwords.push_back( "assistant" );
    listofwords.push_back( "associate" );
    listofwords.push_back( "association" );
    listofwords.push_back( "assumption" );
    listofwords.push_back( "athlete" );
    listofwords.push_back( "athletic" );
    listofwords.push_back( "atmosphere" );
    listofwords.push_back( "attempt" );
    listofwords.push_back( "attention" );
    listofwords.push_back( "attitude" );
    listofwords.push_back( "attorney" );
    listofwords.push_back( "attract" );
    listofwords.push_back( "attractive" );
    listofwords.push_back( "attribute" );
    listofwords.push_back( "audience" );
    listofwords.push_back( "authority" );
    listofwords.push_back( "available" );
    listofwords.push_back( "average" );
    listofwords.push_back( "awareness" );
    listofwords.push_back( "background" );
    listofwords.push_back( "balance" );
    listofwords.push_back( "barrier" );
    listofwords.push_back( "baseball" );
    listofwords.push_back( "basically" );
    listofwords.push_back( "basketball" );
    listofwords.push_back( "bathroom" );
    listofwords.push_back( "battery" );
    listofwords.push_back( "beautiful" );
    listofwords.push_back( "because" );
    listofwords.push_back( "bedroom" );
    listofwords.push_back( "beginning" );
    listofwords.push_back( "behavior" );
    listofwords.push_back( "believe" );
    listofwords.push_back( "beneath" );
    listofwords.push_back( "benefit" );
    listofwords.push_back( "besides" );
    listofwords.push_back( "between" );
    listofwords.push_back( "billion" );
    listofwords.push_back( "biological" );
    listofwords.push_back( "birthday" );
    listofwords.push_back( "blanket" );
    listofwords.push_back( "bombing" );
    listofwords.push_back( "boundary" );
    listofwords.push_back( "boyfriend" );
    listofwords.push_back( "breakfast" );
    listofwords.push_back( "breathe" );
    listofwords.push_back( "briefly" );
    listofwords.push_back( "brilliant" );
    listofwords.push_back( "british" );
    listofwords.push_back( "brother" );
    listofwords.push_back( "building" );
    listofwords.push_back( "business" );
    listofwords.push_back( "cabinet" );
    listofwords.push_back( "calculate" );
    listofwords.push_back( "campaign" );
    listofwords.push_back( "canadian" );
    listofwords.push_back( "candidate" );
    listofwords.push_back( "capability" );
    listofwords.push_back( "capable" );
    listofwords.push_back( "capacity" );
    listofwords.push_back( "capital" );
    listofwords.push_back( "captain" );
    listofwords.push_back( "capture" );
    listofwords.push_back( "careful" );
    listofwords.push_back( "carefully" );
    listofwords.push_back( "carrier" );
    listofwords.push_back( "category" );
    listofwords.push_back( "catholic" );
    listofwords.push_back( "ceiling" );
    listofwords.push_back( "celebrate" );
    listofwords.push_back( "celebration" );
    listofwords.push_back( "celebrity" );
    listofwords.push_back( "central" );
    listofwords.push_back( "century" );
    listofwords.push_back( "ceremony" );
    listofwords.push_back( "certain" );
    listofwords.push_back( "certainly" );
    listofwords.push_back( "chairman" );
    listofwords.push_back( "challenge" );
    listofwords.push_back( "chamber" );
    listofwords.push_back( "champion" );
    listofwords.push_back( "championship" );
    listofwords.push_back( "changing" );
    listofwords.push_back( "channel" );
    listofwords.push_back( "chapter" );
    listofwords.push_back( "character" );
    listofwords.push_back( "characteristic" );
    listofwords.push_back( "characterize" );
    listofwords.push_back( "charity" );
    listofwords.push_back( "chemical" );
    listofwords.push_back( "chicken" );
    listofwords.push_back( "childhood" );
    listofwords.push_back( "chinese" );
    listofwords.push_back( "chocolate" );
    listofwords.push_back( "cholesterol" );
    listofwords.push_back( "christian" );
    listofwords.push_back( "christmas" );
    listofwords.push_back( "cigarette" );
    listofwords.push_back( "circumstance" );
    listofwords.push_back( "citizen" );
    listofwords.push_back( "civilian" );
    listofwords.push_back( "classic" );
    listofwords.push_back( "classroom" );
    listofwords.push_back( "clearly" );
    listofwords.push_back( "climate" );
    listofwords.push_back( "clinical" );
    listofwords.push_back( "closely" );
    listofwords.push_back( "clothes" );
    listofwords.push_back( "clothing" );
    listofwords.push_back( "cluster" );
    listofwords.push_back( "coalition" );
    listofwords.push_back( "cognitive" );
    listofwords.push_back( "collapse" );
    listofwords.push_back( "colleague" );
    listofwords.push_back( "collect" );
    listofwords.push_back( "collection" );
    listofwords.push_back( "collective" );
    listofwords.push_back( "college" );
    listofwords.push_back( "colonial" );
    listofwords.push_back( "combination" );
    listofwords.push_back( "combine" );
    listofwords.push_back( "comfort" );
    listofwords.push_back( "comfortable" );
    listofwords.push_back( "command" );
    listofwords.push_back( "commander" );
    listofwords.push_back( "comment" );
    listofwords.push_back( "commercial" );
    listofwords.push_back( "commission" );
    listofwords.push_back( "commitment" );
    listofwords.push_back( "committee" );
    listofwords.push_back( "communicate" );
    listofwords.push_back( "communication" );
    listofwords.push_back( "community" );
    listofwords.push_back( "company" );
    listofwords.push_back( "compare" );
    listofwords.push_back( "comparison" );
    listofwords.push_back( "compete" );
    listofwords.push_back( "competition" );
    listofwords.push_back( "competitive" );
    listofwords.push_back( "competitor" );
    listofwords.push_back( "complain" );
    listofwords.push_back( "complaint" );
    listofwords.push_back( "complete" );
    listofwords.push_back( "completely" );
    listofwords.push_back( "complex" );
    listofwords.push_back( "complicated" );
    listofwords.push_back( "component" );
    listofwords.push_back( "compose" );
    listofwords.push_back( "composition" );
    listofwords.push_back( "comprehensive" );
    listofwords.push_back( "computer" );
    listofwords.push_back( "concentrate" );
    listofwords.push_back( "concentration" );
    listofwords.push_back( "concept" );
    listofwords.push_back( "concern" );
    listofwords.push_back( "concerned" );
    listofwords.push_back( "concert" );
    listofwords.push_back( "conclude" );
    listofwords.push_back( "conclusion" );
    listofwords.push_back( "concrete" );
    listofwords.push_back( "condition" );
    listofwords.push_back( "conduct" );
    listofwords.push_back( "conference" );
    listofwords.push_back( "confidence" );
    listofwords.push_back( "confident" );
    listofwords.push_back( "confirm" );
    listofwords.push_back( "conflict" );
    listofwords.push_back( "confront" );
    listofwords.push_back( "confusion" );
    listofwords.push_back( "congress" );
    listofwords.push_back( "congressional" );
    listofwords.push_back( "connect" );
    listofwords.push_back( "connection" );
    listofwords.push_back( "consciousness" );
    listofwords.push_back( "consensus" );
    listofwords.push_back( "consequence" );
    listofwords.push_back( "conservative" );
    listofwords.push_back( "consider" );
    listofwords.push_back( "considerable" );
    listofwords.push_back( "consideration" );
    listofwords.push_back( "consist" );
    listofwords.push_back( "consistent" );
    listofwords.push_back( "constant" );
    listofwords.push_back( "constantly" );
    listofwords.push_back( "constitute" );
    listofwords.push_back( "constitutional" );
    listofwords.push_back( "construct" );
    listofwords.push_back( "construction" );
    listofwords.push_back( "consultant" );
    listofwords.push_back( "consume" );
    listofwords.push_back( "consumer" );
    listofwords.push_back( "consumption" );
    listofwords.push_back( "contact" );
    listofwords.push_back( "contain" );
    listofwords.push_back( "container" );
    listofwords.push_back( "contemporary" );
    listofwords.push_back( "content" );
    listofwords.push_back( "contest" );
    listofwords.push_back( "context" );
    listofwords.push_back( "continue" );
    listofwords.push_back( "continued" );
    listofwords.push_back( "contract" );
    listofwords.push_back( "contrast" );
    listofwords.push_back( "contribute" );
    listofwords.push_back( "contribution" );
    listofwords.push_back( "control" );
    listofwords.push_back( "controversial" );
    listofwords.push_back( "controversy" );
    listofwords.push_back( "convention" );
    listofwords.push_back( "conventional" );
    listofwords.push_back( "conversation" );
    listofwords.push_back( "convert" );
    listofwords.push_back( "conviction" );
    listofwords.push_back( "convince" );
    listofwords.push_back( "cooking" );
    listofwords.push_back( "cooperation" );
    listofwords.push_back( "corporate" );
    listofwords.push_back( "corporation" );
    listofwords.push_back( "correct" );
    listofwords.push_back( "correspondent" );
    listofwords.push_back( "council" );
    listofwords.push_back( "counselor" );
    listofwords.push_back( "counter" );
    listofwords.push_back( "country" );
    listofwords.push_back( "courage" );
    listofwords.push_back( "coverage" );
    listofwords.push_back( "creation" );
    listofwords.push_back( "creative" );
    listofwords.push_back( "creature" );
    listofwords.push_back( "criminal" );
    listofwords.push_back( "criteria" );
    listofwords.push_back( "critical" );
    listofwords.push_back( "criticism" );
    listofwords.push_back( "criticize" );
    listofwords.push_back( "crucial" );
    listofwords.push_back( "cultural" );
    listofwords.push_back( "culture" );
    listofwords.push_back( "curious" );
    listofwords.push_back( "current" );
    listofwords.push_back( "currently" );
    listofwords.push_back( "curriculum" );
    listofwords.push_back( "customer" );
    listofwords.push_back( "dangerous" );
    listofwords.push_back( "darkness" );
    listofwords.push_back( "daughter" );
    listofwords.push_back( "decision" );
    listofwords.push_back( "declare" );
    listofwords.push_back( "decline" );
    listofwords.push_back( "decrease" );
    listofwords.push_back( "defendant" );
    listofwords.push_back( "defense" );
    listofwords.push_back( "defensive" );
    listofwords.push_back( "deficit" );
    listofwords.push_back( "definitely" );
    listofwords.push_back( "definition" );
    listofwords.push_back( "deliver" );
    listofwords.push_back( "delivery" );
    listofwords.push_back( "democracy" );
    listofwords.push_back( "democrat" );
    listofwords.push_back( "democratic" );
    listofwords.push_back( "demonstrate" );
    listofwords.push_back( "demonstration" );
    listofwords.push_back( "department" );
    listofwords.push_back( "dependent" );
    listofwords.push_back( "depending" );
    listofwords.push_back( "depression" );
    listofwords.push_back( "describe" );
    listofwords.push_back( "description" );
    listofwords.push_back( "deserve" );
    listofwords.push_back( "designer" );
    listofwords.push_back( "desperate" );
    listofwords.push_back( "despite" );
    listofwords.push_back( "destroy" );
    listofwords.push_back( "destruction" );
    listofwords.push_back( "detailed" );
    listofwords.push_back( "determine" );
    listofwords.push_back( "develop" );
    listofwords.push_back( "developing" );
    listofwords.push_back( "development" );
    listofwords.push_back( "dialogue" );
    listofwords.push_back( "difference" );
    listofwords.push_back( "different" );
    listofwords.push_back( "differently" );
    listofwords.push_back( "difficult" );
    listofwords.push_back( "difficulty" );
    listofwords.push_back( "digital" );
    listofwords.push_back( "dimension" );
    listofwords.push_back( "direction" );
    listofwords.push_back( "directly" );
    listofwords.push_back( "director" );
    listofwords.push_back( "disability" );
    listofwords.push_back( "disagree" );
    listofwords.push_back( "disappear" );
    listofwords.push_back( "disaster" );
    listofwords.push_back( "discipline" );
    listofwords.push_back( "discourse" );
    listofwords.push_back( "discover" );
    listofwords.push_back( "discovery" );
    listofwords.push_back( "discrimination" );
    listofwords.push_back( "discuss" );
    listofwords.push_back( "discussion" );
    listofwords.push_back( "disease" );
    listofwords.push_back( "dismiss" );
    listofwords.push_back( "disorder" );
    listofwords.push_back( "display" );
    listofwords.push_back( "dispute" );
    listofwords.push_back( "distance" );
    listofwords.push_back( "distant" );
    listofwords.push_back( "distinct" );
    listofwords.push_back( "distinction" );
    listofwords.push_back( "distinguish" );
    listofwords.push_back( "distribute" );
    listofwords.push_back( "distribution" );
    listofwords.push_back( "district" );
    listofwords.push_back( "diverse" );
    listofwords.push_back( "diversity" );
    listofwords.push_back( "division" );
    listofwords.push_back( "divorce" );
    listofwords.push_back( "document" );
    listofwords.push_back( "domestic" );
    listofwords.push_back( "dominant" );
    listofwords.push_back( "dominate" );
    listofwords.push_back( "downtown" );
    listofwords.push_back( "dramatic" );
    listofwords.push_back( "dramatically" );
    listofwords.push_back( "drawing" );
    listofwords.push_back( "earnings" );
    listofwords.push_back( "eastern" );
    listofwords.push_back( "economic" );
    listofwords.push_back( "economics" );
    listofwords.push_back( "economist" );
    listofwords.push_back( "economy" );
    listofwords.push_back( "edition" );
    listofwords.push_back( "educate" );
    listofwords.push_back( "education" );
    listofwords.push_back( "educational" );
    listofwords.push_back( "educator" );
    listofwords.push_back( "effective" );
    listofwords.push_back( "effectively" );
    listofwords.push_back( "efficiency" );
    listofwords.push_back( "efficient" );
    listofwords.push_back( "elderly" );
    listofwords.push_back( "election" );
    listofwords.push_back( "electric" );
    listofwords.push_back( "electricity" );
    listofwords.push_back( "electronic" );
    listofwords.push_back( "element" );
    listofwords.push_back( "elementary" );
    listofwords.push_back( "eliminate" );
    listofwords.push_back( "elsewhere" );
    listofwords.push_back( "embrace" );
    listofwords.push_back( "emergency" );
    listofwords.push_back( "emission" );
    listofwords.push_back( "emotion" );
    listofwords.push_back( "emotional" );
    listofwords.push_back( "emphasis" );
    listofwords.push_back( "emphasize" );
    listofwords.push_back( "employee" );
    listofwords.push_back( "employer" );
    listofwords.push_back( "employment" );
    listofwords.push_back( "encounter" );
    listofwords.push_back( "encourage" );
    listofwords.push_back( "enforcement" );
    listofwords.push_back( "engineer" );
    listofwords.push_back( "engineering" );
    listofwords.push_back( "english" );
    listofwords.push_back( "enhance" );
    listofwords.push_back( "enormous" );
    listofwords.push_back( "enterprise" );
    listofwords.push_back( "entertainment" );
    listofwords.push_back( "entirely" );
    listofwords.push_back( "entrance" );
    listofwords.push_back( "environment" );
    listofwords.push_back( "environmental" );
    listofwords.push_back( "episode" );
    listofwords.push_back( "equally" );
    listofwords.push_back( "equipment" );
    listofwords.push_back( "especially" );
    listofwords.push_back( "essential" );
    listofwords.push_back( "essentially" );
    listofwords.push_back( "establish" );
    listofwords.push_back( "establishment" );
    listofwords.push_back( "estimate" );
    listofwords.push_back( "european" );
    listofwords.push_back( "evaluate" );
    listofwords.push_back( "evaluation" );
    listofwords.push_back( "evening" );
    listofwords.push_back( "eventually" );
    listofwords.push_back( "everybody" );
    listofwords.push_back( "everyday" );
    listofwords.push_back( "everyone" );
    listofwords.push_back( "everything" );
    listofwords.push_back( "everywhere" );
    listofwords.push_back( "evidence" );
    listofwords.push_back( "evolution" );
    listofwords.push_back( "exactly" );
    listofwords.push_back( "examination" );
    listofwords.push_back( "examine" );
    listofwords.push_back( "example" );
    listofwords.push_back( "excellent" );
    listofwords.push_back( "exception" );
    listofwords.push_back( "exchange" );
    listofwords.push_back( "exciting" );
    listofwords.push_back( "executive" );
    listofwords.push_back( "exercise" );
    listofwords.push_back( "exhibit" );
    listofwords.push_back( "exhibition" );
    listofwords.push_back( "existence" );
    listofwords.push_back( "existing" );
    listofwords.push_back( "expansion" );
    listofwords.push_back( "expectation" );
    listofwords.push_back( "expense" );
    listofwords.push_back( "expensive" );
    listofwords.push_back( "experience" );
    listofwords.push_back( "experiment" );
    listofwords.push_back( "explain" );
    listofwords.push_back( "explanation" );
    listofwords.push_back( "explode" );
    listofwords.push_back( "explore" );
    listofwords.push_back( "explosion" );
    listofwords.push_back( "exposure" );
    listofwords.push_back( "express" );
    listofwords.push_back( "expression" );
    listofwords.push_back( "extension" );
    listofwords.push_back( "extensive" );
    listofwords.push_back( "external" );
    listofwords.push_back( "extraordinary" );
    listofwords.push_back( "extreme" );
    listofwords.push_back( "extremely" );
    listofwords.push_back( "facility" );
    listofwords.push_back( "factory" );
    listofwords.push_back( "faculty" );
    listofwords.push_back( "failure" );
    listofwords.push_back( "familiar" );
    listofwords.push_back( "fantasy" );
    listofwords.push_back( "fashion" );
    listofwords.push_back( "favorite" );
    listofwords.push_back( "feature" );
    listofwords.push_back( "federal" );
    listofwords.push_back( "feeling" );
    listofwords.push_back( "fiction" );
    listofwords.push_back( "fifteen" );
    listofwords.push_back( "fighter" );
    listofwords.push_back( "fighting" );
    listofwords.push_back( "finally" );
    listofwords.push_back( "finance" );
    listofwords.push_back( "financial" );
    listofwords.push_back( "finding" );
    listofwords.push_back( "fishing" );
    listofwords.push_back( "fitness" );
    listofwords.push_back( "following" );
    listofwords.push_back( "football" );
    listofwords.push_back( "foreign" );
    listofwords.push_back( "forever" );
    listofwords.push_back( "formation" );
    listofwords.push_back( "formula" );
    listofwords.push_back( "fortune" );
    listofwords.push_back( "forward" );
    listofwords.push_back( "foundation" );
    listofwords.push_back( "founder" );
    listofwords.push_back( "framework" );
    listofwords.push_back( "freedom" );
    listofwords.push_back( "frequency" );
    listofwords.push_back( "frequent" );
    listofwords.push_back( "frequently" );
    listofwords.push_back( "friendly" );
    listofwords.push_back( "friendship" );
    listofwords.push_back( "frustration" );
    listofwords.push_back( "function" );
    listofwords.push_back( "fundamental" );
    listofwords.push_back( "funding" );
    listofwords.push_back( "funeral" );
    listofwords.push_back( "furniture" );
    listofwords.push_back( "furthermore" );
    listofwords.push_back( "gallery" );
    listofwords.push_back( "general" );
    listofwords.push_back( "generally" );
    listofwords.push_back( "generate" );
    listofwords.push_back( "generation" );
    listofwords.push_back( "genetic" );
    listofwords.push_back( "gentleman" );
    listofwords.push_back( "gesture" );
    listofwords.push_back( "girlfriend" );
    listofwords.push_back( "government" );
    listofwords.push_back( "governor" );
    listofwords.push_back( "gradually" );
    listofwords.push_back( "graduate" );
    listofwords.push_back( "grandfather" );
    listofwords.push_back( "grandmother" );
    listofwords.push_back( "greatest" );
    listofwords.push_back( "grocery" );
    listofwords.push_back( "growing" );
    listofwords.push_back( "guarantee" );
    listofwords.push_back( "guideline" );
    listofwords.push_back( "habitat" );
    listofwords.push_back( "handful" );
    listofwords.push_back( "headline" );
    listofwords.push_back( "headquarters" );
    listofwords.push_back( "healthy" );
    listofwords.push_back( "hearing" );
    listofwords.push_back( "heavily" );
    listofwords.push_back( "helicopter" );
    listofwords.push_back( "helpful" );
    listofwords.push_back( "heritage" );
    listofwords.push_back( "herself" );
    listofwords.push_back( "highlight" );
    listofwords.push_back( "highway" );
    listofwords.push_back( "himself" );
    listofwords.push_back( "historian" );
    listofwords.push_back( "historic" );
    listofwords.push_back( "historical" );
    listofwords.push_back( "history" );
    listofwords.push_back( "holiday" );
    listofwords.push_back( "homeless" );
    listofwords.push_back( "horizon" );
    listofwords.push_back( "hospital" );
    listofwords.push_back( "household" );
    listofwords.push_back( "housing" );
    listofwords.push_back( "however" );
    listofwords.push_back( "hundred" );
    listofwords.push_back( "hunting" );
    listofwords.push_back( "husband" );
    listofwords.push_back( "hypothesis" );
    listofwords.push_back( "identification" );
    listofwords.push_back( "identify" );
    listofwords.push_back( "identity" );
    listofwords.push_back( "illegal" );
    listofwords.push_back( "illness" );
    listofwords.push_back( "illustrate" );
    listofwords.push_back( "imagination" );
    listofwords.push_back( "imagine" );
    listofwords.push_back( "immediate" );
    listofwords.push_back( "immediately" );
    listofwords.push_back( "immigrant" );
    listofwords.push_back( "immigration" );
    listofwords.push_back( "implement" );
    listofwords.push_back( "implication" );
    listofwords.push_back( "importance" );
    listofwords.push_back( "important" );
    listofwords.push_back( "impossible" );
    listofwords.push_back( "impress" );
    listofwords.push_back( "impression" );
    listofwords.push_back( "impressive" );
    listofwords.push_back( "improve" );
    listofwords.push_back( "improvement" );
    listofwords.push_back( "incentive" );
    listofwords.push_back( "incident" );
    listofwords.push_back( "include" );
    listofwords.push_back( "including" );
    listofwords.push_back( "incorporate" );
    listofwords.push_back( "increase" );
    listofwords.push_back( "increased" );
    listofwords.push_back( "increasing" );
    listofwords.push_back( "increasingly" );
    listofwords.push_back( "incredible" );
    listofwords.push_back( "independence" );
    listofwords.push_back( "independent" );
    listofwords.push_back( "indicate" );
    listofwords.push_back( "indication" );
    listofwords.push_back( "individual" );
    listofwords.push_back( "industrial" );
    listofwords.push_back( "industry" );
    listofwords.push_back( "infection" );
    listofwords.push_back( "inflation" );
    listofwords.push_back( "influence" );
    listofwords.push_back( "information" );
    listofwords.push_back( "ingredient" );
    listofwords.push_back( "initial" );
    listofwords.push_back( "initially" );
    listofwords.push_back( "initiative" );
    listofwords.push_back( "innocent" );
    listofwords.push_back( "inquiry" );
    listofwords.push_back( "insight" );
    listofwords.push_back( "inspire" );
    listofwords.push_back( "install" );
    listofwords.push_back( "instance" );
    listofwords.push_back( "instead" );
    listofwords.push_back( "institution" );
    listofwords.push_back( "institutional" );
    listofwords.push_back( "instruction" );
    listofwords.push_back( "instructor" );
    listofwords.push_back( "instrument" );
    listofwords.push_back( "insurance" );
    listofwords.push_back( "intellectual" );
    listofwords.push_back( "intelligence" );
    listofwords.push_back( "intense" );
    listofwords.push_back( "intensity" );
    listofwords.push_back( "intention" );
    listofwords.push_back( "interaction" );
    listofwords.push_back( "interest" );
    listofwords.push_back( "interested" );
    listofwords.push_back( "interesting" );
    listofwords.push_back( "internal" );
    listofwords.push_back( "international" );
    listofwords.push_back( "internet" );
    listofwords.push_back( "interpret" );
    listofwords.push_back( "interpretation" );
    listofwords.push_back( "intervention" );
    listofwords.push_back( "interview" );
    listofwords.push_back( "introduce" );
    listofwords.push_back( "introduction" );
    listofwords.push_back( "invasion" );
    listofwords.push_back( "investigate" );
    listofwords.push_back( "investigation" );
    listofwords.push_back( "investigator" );
    listofwords.push_back( "investment" );
    listofwords.push_back( "investor" );
    listofwords.push_back( "involve" );
    listofwords.push_back( "involved" );
    listofwords.push_back( "involvement" );
    listofwords.push_back( "islamic" );
    listofwords.push_back( "israeli" );
    listofwords.push_back( "italian" );
    listofwords.push_back( "japanese" );
    listofwords.push_back( "journal" );
    listofwords.push_back( "journalist" );
    listofwords.push_back( "journey" );
    listofwords.push_back( "judgment" );
    listofwords.push_back( "justice" );
    listofwords.push_back( "justify" );
    listofwords.push_back( "killing" );
    listofwords.push_back( "kitchen" );
    listofwords.push_back( "knowledge" );
    listofwords.push_back( "laboratory" );
    listofwords.push_back( "landscape" );
    listofwords.push_back( "language" );
    listofwords.push_back( "largely" );
    listofwords.push_back( "lawsuit" );
    listofwords.push_back( "leadership" );
    listofwords.push_back( "leading" );
    listofwords.push_back( "learning" );
    listofwords.push_back( "leather" );
    listofwords.push_back( "legislation" );
    listofwords.push_back( "legitimate" );
    listofwords.push_back( "liberal" );
    listofwords.push_back( "library" );
    listofwords.push_back( "license" );
    listofwords.push_back( "lifestyle" );
    listofwords.push_back( "lifetime" );
    listofwords.push_back( "limitation" );
    listofwords.push_back( "limited" );
    listofwords.push_back( "literally" );
    listofwords.push_back( "literary" );
    listofwords.push_back( "literature" );
    listofwords.push_back( "location" );
    listofwords.push_back( "long-term" );
    listofwords.push_back( "machine" );
    listofwords.push_back( "magazine" );
    listofwords.push_back( "maintain" );
    listofwords.push_back( "maintenance" );
    listofwords.push_back( "majority" );
    listofwords.push_back( "management" );
    listofwords.push_back( "manager" );
    listofwords.push_back( "manufacturer" );
    listofwords.push_back( "manufacturing" );
    listofwords.push_back( "marketing" );
    listofwords.push_back( "marriage" );
    listofwords.push_back( "married" );
    listofwords.push_back( "massive" );
    listofwords.push_back( "material" );
    listofwords.push_back( "meaning" );
    listofwords.push_back( "meanwhile" );
    listofwords.push_back( "measure" );
    listofwords.push_back( "measurement" );
    listofwords.push_back( "mechanism" );
    listofwords.push_back( "medical" );
    listofwords.push_back( "medication" );
    listofwords.push_back( "medicine" );
    listofwords.push_back( "meeting" );
    listofwords.push_back( "membership" );
    listofwords.push_back( "mention" );
    listofwords.push_back( "message" );
    listofwords.push_back( "mexican" );
    listofwords.push_back( "military" );
    listofwords.push_back( "million" );
    listofwords.push_back( "minister" );
    listofwords.push_back( "minority" );
    listofwords.push_back( "miracle" );
    listofwords.push_back( "missile" );
    listofwords.push_back( "mission" );
    listofwords.push_back( "mistake" );
    listofwords.push_back( "mixture" );
    listofwords.push_back( "moderate" );
    listofwords.push_back( "monitor" );
    listofwords.push_back( "moreover" );
    listofwords.push_back( "morning" );
    listofwords.push_back( "mortgage" );
    listofwords.push_back( "motivation" );
    listofwords.push_back( "mountain" );
    listofwords.push_back( "movement" );
    listofwords.push_back( "multiple" );
    listofwords.push_back( "musical" );
    listofwords.push_back( "musician" );
    listofwords.push_back( "mystery" );
    listofwords.push_back( "narrative" );
    listofwords.push_back( "national" );
    listofwords.push_back( "natural" );
    listofwords.push_back( "naturally" );
    listofwords.push_back( "necessarily" );
    listofwords.push_back( "necessary" );
    listofwords.push_back( "negative" );
    listofwords.push_back( "negotiate" );
    listofwords.push_back( "negotiation" );
    listofwords.push_back( "neighbor" );
    listofwords.push_back( "neighborhood" );
    listofwords.push_back( "neither" );
    listofwords.push_back( "nervous" );
    listofwords.push_back( "network" );
    listofwords.push_back( "nevertheless" );
    listofwords.push_back( "newspaper" );
    listofwords.push_back( "nomination" );
    listofwords.push_back( "nonetheless" );
    listofwords.push_back( "normally" );
    listofwords.push_back( "northern" );
    listofwords.push_back( "nothing" );
    listofwords.push_back( "nowhere" );
    listofwords.push_back( "nuclear" );
    listofwords.push_back( "numerous" );
    listofwords.push_back( "objective" );
    listofwords.push_back( "obligation" );
    listofwords.push_back( "observation" );
    listofwords.push_back( "observe" );
    listofwords.push_back( "observer" );
    listofwords.push_back( "obvious" );
    listofwords.push_back( "obviously" );
    listofwords.push_back( "occasion" );
    listofwords.push_back( "occasionally" );
    listofwords.push_back( "occupation" );
    listofwords.push_back( "offense" );
    listofwords.push_back( "offensive" );
    listofwords.push_back( "officer" );
    listofwords.push_back( "official" );
    listofwords.push_back( "olympic" );
    listofwords.push_back( "ongoing" );
    listofwords.push_back( "opening" );
    listofwords.push_back( "operate" );
    listofwords.push_back( "operating" );
    listofwords.push_back( "operation" );
    listofwords.push_back( "operator" );
    listofwords.push_back( "opinion" );
    listofwords.push_back( "opponent" );
    listofwords.push_back( "opportunity" );
    listofwords.push_back( "opposite" );
    listofwords.push_back( "opposition" );
    listofwords.push_back( "ordinary" );
    listofwords.push_back( "organic" );
    listofwords.push_back( "organization" );
    listofwords.push_back( "organize" );
    listofwords.push_back( "orientation" );
    listofwords.push_back( "original" );
    listofwords.push_back( "originally" );
    listofwords.push_back( "otherwise" );
    listofwords.push_back( "ourselves" );
    listofwords.push_back( "outcome" );
    listofwords.push_back( "outside" );
    listofwords.push_back( "overall" );
    listofwords.push_back( "overcome" );
    listofwords.push_back( "overlook" );
    listofwords.push_back( "package" );
    listofwords.push_back( "painful" );
    listofwords.push_back( "painter" );
    listofwords.push_back( "painting" );
    listofwords.push_back( "palestinian" );
    listofwords.push_back( "parking" );
    listofwords.push_back( "participant" );
    listofwords.push_back( "participate" );
    listofwords.push_back( "participation" );
    listofwords.push_back( "particular" );
    listofwords.push_back( "particularly" );
    listofwords.push_back( "partner" );
    listofwords.push_back( "partnership" );
    listofwords.push_back( "passage" );
    listofwords.push_back( "passenger" );
    listofwords.push_back( "passion" );
    listofwords.push_back( "patient" );
    listofwords.push_back( "pattern" );
    listofwords.push_back( "payment" );
    listofwords.push_back( "penalty" );
    listofwords.push_back( "perceive" );
    listofwords.push_back( "percentage" );
    listofwords.push_back( "perception" );
    listofwords.push_back( "perfect" );
    listofwords.push_back( "perfectly" );
    listofwords.push_back( "perform" );
    listofwords.push_back( "performance" );
    listofwords.push_back( "perhaps" );
    listofwords.push_back( "permanent" );
    listofwords.push_back( "permission" );
    listofwords.push_back( "personal" );
    listofwords.push_back( "personality" );
    listofwords.push_back( "personally" );
    listofwords.push_back( "personnel" );
    listofwords.push_back( "perspective" );
    listofwords.push_back( "persuade" );
    listofwords.push_back( "phenomenon" );
    listofwords.push_back( "philosophy" );
    listofwords.push_back( "photograph" );
    listofwords.push_back( "photographer" );
    listofwords.push_back( "physical" );
    listofwords.push_back( "physically" );
    listofwords.push_back( "physician" );
    listofwords.push_back( "picture" );
    listofwords.push_back( "planning" );
    listofwords.push_back( "plastic" );
    listofwords.push_back( "platform" );
    listofwords.push_back( "pleasure" );
    listofwords.push_back( "political" );
    listofwords.push_back( "politically" );
    listofwords.push_back( "politician" );
    listofwords.push_back( "politics" );
    listofwords.push_back( "pollution" );
    listofwords.push_back( "popular" );
    listofwords.push_back( "population" );
    listofwords.push_back( "portion" );
    listofwords.push_back( "portrait" );
    listofwords.push_back( "portray" );
    listofwords.push_back( "position" );
    listofwords.push_back( "positive" );
    listofwords.push_back( "possess" );
    listofwords.push_back( "possibility" );
    listofwords.push_back( "possible" );
    listofwords.push_back( "possibly" );
    listofwords.push_back( "potential" );
    listofwords.push_back( "potentially" );
    listofwords.push_back( "poverty" );
    listofwords.push_back( "powerful" );
    listofwords.push_back( "practical" );
    listofwords.push_back( "practice" );
    listofwords.push_back( "precisely" );
    listofwords.push_back( "predict" );
    listofwords.push_back( "preference" );
    listofwords.push_back( "pregnancy" );
    listofwords.push_back( "pregnant" );
    listofwords.push_back( "preparation" );
    listofwords.push_back( "prepare" );
    listofwords.push_back( "prescription" );
    listofwords.push_back( "presence" );
    listofwords.push_back( "present" );
    listofwords.push_back( "presentation" );
    listofwords.push_back( "preserve" );
    listofwords.push_back( "president" );
    listofwords.push_back( "presidential" );
    listofwords.push_back( "pressure" );
    listofwords.push_back( "pretend" );
    listofwords.push_back( "prevent" );
    listofwords.push_back( "previous" );
    listofwords.push_back( "previously" );
    listofwords.push_back( "primarily" );
    listofwords.push_back( "primary" );
    listofwords.push_back( "principal" );
    listofwords.push_back( "principle" );
    listofwords.push_back( "priority" );
    listofwords.push_back( "prisoner" );
    listofwords.push_back( "privacy" );
    listofwords.push_back( "private" );
    listofwords.push_back( "probably" );
    listofwords.push_back( "problem" );
    listofwords.push_back( "procedure" );
    listofwords.push_back( "proceed" );
    listofwords.push_back( "process" );
    listofwords.push_back( "produce" );
    listofwords.push_back( "producer" );
    listofwords.push_back( "product" );
    listofwords.push_back( "production" );
    listofwords.push_back( "profession" );
    listofwords.push_back( "professional" );
    listofwords.push_back( "professor" );
    listofwords.push_back( "profile" );
    listofwords.push_back( "program" );
    listofwords.push_back( "progress" );
    listofwords.push_back( "project" );
    listofwords.push_back( "prominent" );
    listofwords.push_back( "promise" );
    listofwords.push_back( "promote" );
    listofwords.push_back( "properly" );
    listofwords.push_back( "property" );
    listofwords.push_back( "proportion" );
    listofwords.push_back( "proposal" );
    listofwords.push_back( "propose" );
    listofwords.push_back( "proposed" );
    listofwords.push_back( "prosecutor" );
    listofwords.push_back( "prospect" );
    listofwords.push_back( "protect" );
    listofwords.push_back( "protection" );
    listofwords.push_back( "protein" );
    listofwords.push_back( "protest" );
    listofwords.push_back( "provide" );
    listofwords.push_back( "provider" );
    listofwords.push_back( "province" );
    listofwords.push_back( "provision" );
    listofwords.push_back( "psychological" );
    listofwords.push_back( "psychologist" );
    listofwords.push_back( "psychology" );
    listofwords.push_back( "publication" );
    listofwords.push_back( "publicly" );
    listofwords.push_back( "publish" );
    listofwords.push_back( "publisher" );
    listofwords.push_back( "punishment" );
    listofwords.push_back( "purchase" );
    listofwords.push_back( "purpose" );
    listofwords.push_back( "qualify" );
    listofwords.push_back( "quality" );
    listofwords.push_back( "quarter" );
    listofwords.push_back( "quarterback" );
    listofwords.push_back( "question" );
    listofwords.push_back( "quickly" );
    listofwords.push_back( "quietly" );
    listofwords.push_back( "radical" );
    listofwords.push_back( "rapidly" );
    listofwords.push_back( "reaction" );
    listofwords.push_back( "reading" );
    listofwords.push_back( "reality" );
    listofwords.push_back( "realize" );
    listofwords.push_back( "reasonable" );
    listofwords.push_back( "receive" );
    listofwords.push_back( "recently" );
    listofwords.push_back( "recognition" );
    listofwords.push_back( "recognize" );
    listofwords.push_back( "recommend" );
    listofwords.push_back( "recommendation" );
    listofwords.push_back( "recording" );
    listofwords.push_back( "recover" );
    listofwords.push_back( "recovery" );
    listofwords.push_back( "recruit" );
    listofwords.push_back( "reduction" );
    listofwords.push_back( "reference" );
    listofwords.push_back( "reflect" );
    listofwords.push_back( "reflection" );
    listofwords.push_back( "refugee" );
    listofwords.push_back( "regarding" );
    listofwords.push_back( "regardless" );
    listofwords.push_back( "regional" );
    listofwords.push_back( "register" );
    listofwords.push_back( "regular" );
    listofwords.push_back( "regularly" );
    listofwords.push_back( "regulate" );
    listofwords.push_back( "regulation" );
    listofwords.push_back( "reinforce" );
    listofwords.push_back( "relation" );
    listofwords.push_back( "relationship" );
    listofwords.push_back( "relative" );
    listofwords.push_back( "relatively" );
    listofwords.push_back( "release" );
    listofwords.push_back( "relevant" );
    listofwords.push_back( "religion" );
    listofwords.push_back( "religious" );
    listofwords.push_back( "remaining" );
    listofwords.push_back( "remarkable" );
    listofwords.push_back( "remember" );
    listofwords.push_back( "repeatedly" );
    listofwords.push_back( "replace" );
    listofwords.push_back( "reporter" );
    listofwords.push_back( "represent" );
    listofwords.push_back( "representation" );
    listofwords.push_back( "representative" );
    listofwords.push_back( "republican" );
    listofwords.push_back( "reputation" );
    listofwords.push_back( "request" );
    listofwords.push_back( "require" );
    listofwords.push_back( "requirement" );
    listofwords.push_back( "research" );
    listofwords.push_back( "researcher" );
    listofwords.push_back( "resemble" );
    listofwords.push_back( "reservation" );
    listofwords.push_back( "resident" );
    listofwords.push_back( "resistance" );
    listofwords.push_back( "resolution" );
    listofwords.push_back( "resolve" );
    listofwords.push_back( "resource" );
    listofwords.push_back( "respect" );
    listofwords.push_back( "respond" );
    listofwords.push_back( "respondent" );
    listofwords.push_back( "response" );
    listofwords.push_back( "responsibility" );
    listofwords.push_back( "responsible" );
    listofwords.push_back( "restaurant" );
    listofwords.push_back( "restore" );
    listofwords.push_back( "restriction" );
    listofwords.push_back( "retirement" );
    listofwords.push_back( "revenue" );
    listofwords.push_back( "revolution" );
    listofwords.push_back( "romantic" );
    listofwords.push_back( "roughly" );
    listofwords.push_back( "routine" );
    listofwords.push_back( "running" );
    listofwords.push_back( "russian" );
    listofwords.push_back( "sanction" );
    listofwords.push_back( "satellite" );
    listofwords.push_back( "satisfaction" );
    listofwords.push_back( "satisfy" );
    listofwords.push_back( "scandal" );
    listofwords.push_back( "scenario" );
    listofwords.push_back( "schedule" );
    listofwords.push_back( "scholar" );
    listofwords.push_back( "scholarship" );
    listofwords.push_back( "science" );
    listofwords.push_back( "scientific" );
    listofwords.push_back( "scientist" );
    listofwords.push_back( "secretary" );
    listofwords.push_back( "section" );
    listofwords.push_back( "security" );
    listofwords.push_back( "segment" );
    listofwords.push_back( "selection" );
    listofwords.push_back( "senator" );
    listofwords.push_back( "sensitive" );
    listofwords.push_back( "sentence" );
    listofwords.push_back( "separate" );
    listofwords.push_back( "sequence" );
    listofwords.push_back( "serious" );
    listofwords.push_back( "seriously" );
    listofwords.push_back( "service" );
    listofwords.push_back( "session" );
    listofwords.push_back( "setting" );
    listofwords.push_back( "settlement" );
    listofwords.push_back( "several" );
    listofwords.push_back( "shelter" );
    listofwords.push_back( "shooting" );
    listofwords.push_back( "shopping" );
    listofwords.push_back( "shortly" );
    listofwords.push_back( "shoulder" );
    listofwords.push_back( "significance" );
    listofwords.push_back( "significant" );
    listofwords.push_back( "significantly" );
    listofwords.push_back( "silence" );
    listofwords.push_back( "similar" );
    listofwords.push_back( "similarly" );
    listofwords.push_back( "situation" );
    listofwords.push_back( "slightly" );
    listofwords.push_back( "so-called" );
    listofwords.push_back( "society" );
    listofwords.push_back( "software" );
    listofwords.push_back( "soldier" );
    listofwords.push_back( "solution" );
    listofwords.push_back( "somebody" );
    listofwords.push_back( "somehow" );
    listofwords.push_back( "someone" );
    listofwords.push_back( "something" );
    listofwords.push_back( "sometimes" );
    listofwords.push_back( "somewhat" );
    listofwords.push_back( "somewhere" );
    listofwords.push_back( "sophisticated" );
    listofwords.push_back( "southern" );
    listofwords.push_back( "spanish" );
    listofwords.push_back( "speaker" );
    listofwords.push_back( "special" );
    listofwords.push_back( "specialist" );
    listofwords.push_back( "species" );
    listofwords.push_back( "specific" );
    listofwords.push_back( "specifically" );
    listofwords.push_back( "spending" );
    listofwords.push_back( "spiritual" );
    listofwords.push_back( "spokesman" );
    listofwords.push_back( "squeeze" );
    listofwords.push_back( "stability" );
    listofwords.push_back( "standard" );
    listofwords.push_back( "standing" );
    listofwords.push_back( "statement" );
    listofwords.push_back( "station" );
    listofwords.push_back( "statistics" );
    listofwords.push_back( "stomach" );
    listofwords.push_back( "storage" );
    listofwords.push_back( "straight" );
    listofwords.push_back( "strange" );
    listofwords.push_back( "stranger" );
    listofwords.push_back( "strategic" );
    listofwords.push_back( "strategy" );
    listofwords.push_back( "strength" );
    listofwords.push_back( "strengthen" );
    listofwords.push_back( "stretch" );
    listofwords.push_back( "strongly" );
    listofwords.push_back( "structure" );
    listofwords.push_back( "struggle" );
    listofwords.push_back( "student" );
    listofwords.push_back( "subject" );
    listofwords.push_back( "subsequent" );
    listofwords.push_back( "substance" );
    listofwords.push_back( "substantial" );
    listofwords.push_back( "succeed" );
    listofwords.push_back( "success" );
    listofwords.push_back( "successful" );
    listofwords.push_back( "successfully" );
    listofwords.push_back( "suddenly" );
    listofwords.push_back( "sufficient" );
    listofwords.push_back( "suggest" );
    listofwords.push_back( "suggestion" );
    listofwords.push_back( "suicide" );
    listofwords.push_back( "support" );
    listofwords.push_back( "supporter" );
    listofwords.push_back( "suppose" );
    listofwords.push_back( "supposed" );
    listofwords.push_back( "supreme" );
    listofwords.push_back( "surface" );
    listofwords.push_back( "surgery" );
    listofwords.push_back( "surprise" );
    listofwords.push_back( "surprised" );
    listofwords.push_back( "surprising" );
    listofwords.push_back( "surprisingly" );
    listofwords.push_back( "surround" );
    listofwords.push_back( "survival" );
    listofwords.push_back( "survive" );
    listofwords.push_back( "survivor" );
    listofwords.push_back( "suspect" );
    listofwords.push_back( "sustain" );
    listofwords.push_back( "symptom" );
    listofwords.push_back( "tablespoon" );
    listofwords.push_back( "taxpayer" );
    listofwords.push_back( "teacher" );
    listofwords.push_back( "teaching" );
    listofwords.push_back( "teaspoon" );
    listofwords.push_back( "technical" );
    listofwords.push_back( "technique" );
    listofwords.push_back( "technology" );
    listofwords.push_back( "teenager" );
    listofwords.push_back( "telephone" );
    listofwords.push_back( "telescope" );
    listofwords.push_back( "television" );
    listofwords.push_back( "temperature" );
    listofwords.push_back( "temporary" );
    listofwords.push_back( "tendency" );
    listofwords.push_back( "tension" );
    listofwords.push_back( "terrible" );
    listofwords.push_back( "territory" );
    listofwords.push_back( "terrorism" );
    listofwords.push_back( "terrorist" );
    listofwords.push_back( "testify" );
    listofwords.push_back( "testimony" );
    listofwords.push_back( "testing" );
    listofwords.push_back( "theater" );
    listofwords.push_back( "themselves" );
    listofwords.push_back( "therapy" );
    listofwords.push_back( "therefore" );
    listofwords.push_back( "thinking" );
    listofwords.push_back( "thought" );
    listofwords.push_back( "thousand" );
    listofwords.push_back( "threaten" );
    listofwords.push_back( "through" );
    listofwords.push_back( "throughout" );
    listofwords.push_back( "tobacco" );
    listofwords.push_back( "together" );
    listofwords.push_back( "tomorrow" );
    listofwords.push_back( "tonight" );
    listofwords.push_back( "totally" );
    listofwords.push_back( "tourist" );
    listofwords.push_back( "tournament" );
    listofwords.push_back( "towards" );
    listofwords.push_back( "tradition" );
    listofwords.push_back( "traditional" );
    listofwords.push_back( "traffic" );
    listofwords.push_back( "tragedy" );
    listofwords.push_back( "training" );
    listofwords.push_back( "transfer" );
    listofwords.push_back( "transform" );
    listofwords.push_back( "transformation" );
    listofwords.push_back( "transition" );
    listofwords.push_back( "translate" );
    listofwords.push_back( "transportation" );
    listofwords.push_back( "treatment" );
    listofwords.push_back( "tremendous" );
    listofwords.push_back( "trouble" );
    listofwords.push_back( "typical" );
    listofwords.push_back( "typically" );
    listofwords.push_back( "ultimate" );
    listofwords.push_back( "ultimately" );
    listofwords.push_back( "undergo" );
    listofwords.push_back( "understand" );
    listofwords.push_back( "understanding" );
    listofwords.push_back( "unfortunately" );
    listofwords.push_back( "uniform" );
    listofwords.push_back( "universal" );
    listofwords.push_back( "universe" );
    listofwords.push_back( "university" );
    listofwords.push_back( "unknown" );
    listofwords.push_back( "unlikely" );
    listofwords.push_back( "unusual" );
    listofwords.push_back( "usually" );
    listofwords.push_back( "utility" );
    listofwords.push_back( "vacation" );
    listofwords.push_back( "valuable" );
    listofwords.push_back( "variable" );
    listofwords.push_back( "variation" );
    listofwords.push_back( "variety" );
    listofwords.push_back( "various" );
    listofwords.push_back( "vegetable" );
    listofwords.push_back( "vehicle" );
    listofwords.push_back( "venture" );
    listofwords.push_back( "version" );
    listofwords.push_back( "veteran" );
    listofwords.push_back( "victory" );
    listofwords.push_back( "village" );
    listofwords.push_back( "violate" );
    listofwords.push_back( "violation" );
    listofwords.push_back( "violence" );
    listofwords.push_back( "violent" );
    listofwords.push_back( "virtually" );
    listofwords.push_back( "visible" );
    listofwords.push_back( "visitor" );
    listofwords.push_back( "volunteer" );
    listofwords.push_back( "vulnerable" );
    listofwords.push_back( "warning" );
    listofwords.push_back( "wealthy" );
    listofwords.push_back( "weather" );
    listofwords.push_back( "wedding" );
    listofwords.push_back( "weekend" );
    listofwords.push_back( "welcome" );
    listofwords.push_back( "welfare" );
    listofwords.push_back( "western" );
    listofwords.push_back( "whatever" );
    listofwords.push_back( "whenever" );
    listofwords.push_back( "whereas" );
    listofwords.push_back( "whether" );
    listofwords.push_back( "whisper" );
    listofwords.push_back( "willing" );
    listofwords.push_back( "withdraw" );
    listofwords.push_back( "without" );
    listofwords.push_back( "witness" );
    listofwords.push_back( "wonderful" );
    listofwords.push_back( "working" );
    listofwords.push_back( "workshop" );
    listofwords.push_back( "worried" );
    listofwords.push_back( "writing" );
    listofwords.push_back( "yesterday" );
    listofwords.push_back( "yourself" );



    rand:
    int random=rand()%listofwords.size();
    generatedword=listofwords[random];
    wordlength=generatedword.length();

    if(level==1)
       {
            if((wordlength<7) || (wordlength>12)) goto rand;
       }
   if(level==2)
       {
            if(wordlength<6 || wordlength>12) goto rand;
       }
    if(level==3)
       {
            if((wordlength<5) || (wordlength>12)) goto rand;
       }
}


Guessmyword::~Guessmyword()
{
    delete ui;
}
