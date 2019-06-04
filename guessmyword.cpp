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
            if(level==1 && i==0 && gamecounter==0)    //statment for easy level
            {
                tableword+=generatedword[i]+" ";
                ifexist=true;
                letterexist=true;
                break;
            }
            else if(level==2 && i==wordlength-1 && gamecounter==0)    //statement for normal level
            {
                tableword+=generatedword[i]+" ";
                ifexist=true;
                letterexist=true;
                break;
            }
            if(generatedletters[j]==generatedword[i])   //casual statment
            {
                tableword+=generatedword[i]+" ";
                ifexist=true;
                letterexist=true;
                break;
            }
        }
         if(ifexist==false) tableword+="_ ";
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
                ui->commentator->setText("Seems we have drawn, how about rematch?");
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
            QString currentword=listofwords[i];
            for(int j=0;j<currentword.length();j++)
               {
                    if(currentword.at(j)==s1)
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
    else if(listofwords.size()<10)
    {
        wordlisthaslessthantenelements=true;
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

     char words[2290][20] {"abandon" ,
                           "ability" ,
                           "abortion" ,
                           "about" ,
                           "above" ,
                           "abroad" ,
                           "absence" ,
                           "absolute" ,
                           "absolutely" ,
                           "absorb" ,
                           "abuse" ,
                           "academic" ,
                           "accept" ,
                           "access" ,
                           "accident" ,
                           "accompany" ,
                           "accomplish" ,
                           "according" ,
                           "account" ,
                           "accurate" ,
                           "accuse" ,
                           "achieve" ,
                           "achievement" ,
                           "acknowledge" ,
                           "acquire" ,
                           "across" ,
                           "action" ,
                           "active" ,
                           "activist" ,
                           "activity" ,
                           "actor" ,
                           "actress" ,
                           "actual" ,
                           "actually" ,
                           "adapt" ,
                           "addition" ,
                           "additional" ,
                           "address" ,
                           "adequate" ,
                           "adjust" ,
                           "adjustment" ,
                           "administration" ,
                           "administrator" ,
                           "admire" ,
                           "admission" ,
                           "admit" ,
                           "adolescent" ,
                           "adopt" ,
                           "adult" ,
                           "advance" ,
                           "advanced" ,
                           "advantage" ,
                           "adventure" ,
                           "advertising" ,
                           "advice" ,
                           "advise" ,
                           "adviser" ,
                           "advocate" ,
                           "affair" ,
                           "affect" ,
                           "afford" ,
                           "afraid" ,
                           "African" ,
                           "African-American" ,
                           "after" ,
                           "afternoon" ,
                           "again" ,
                           "against" ,
                           "agency" ,
                           "agenda" ,
                           "agent" ,
                           "aggressive" ,
                           "agree" ,
                           "agreement" ,
                           "agricultural" ,
                           "ahead" ,
                           "aircraft" ,
                           "airline" ,
                           "airport" ,
                           "album" ,
                           "alcohol" ,
                           "alive" ,
                           "alliance" ,
                           "allow" ,
                           "almost" ,
                           "alone" ,
                           "along" ,
                           "already" ,
                           "alter" ,
                           "alternative" ,
                           "although" ,
                           "always" ,
                           "amazing" ,
                           "American" ,
                           "among" ,
                           "amount" ,
                           "analysis" ,
                           "analyst" ,
                           "analyze" ,
                           "ancient" ,
                           "anger" ,
                           "angle" ,
                           "angry" ,
                           "animal" ,
                           "anniversary" ,
                           "announce" ,
                           "annual" ,
                           "another" ,
                           "answer" ,
                           "anticipate" ,
                           "anxiety" ,
                           "anybody" ,
                           "anymore" ,
                           "anyone" ,
                           "anything" ,
                           "anyway" ,
                           "anywhere" ,
                           "apart" ,
                           "apartment" ,
                           "apparent" ,
                           "apparently" ,
                           "appeal" ,
                           "appear" ,
                           "appearance" ,
                           "apple" ,
                           "application" ,
                           "apply" ,
                           "appoint" ,
                           "appointment" ,
                           "appreciate" ,
                           "approach" ,
                           "appropriate" ,
                           "approval" ,
                           "approve" ,
                           "approximately" ,
                           "architect" ,
                           "argue" ,
                           "argument" ,
                           "arise" ,
                           "armed" ,
                           "around" ,
                           "arrange" ,
                           "arrangement" ,
                           "arrest" ,
                           "arrival" ,
                           "arrive" ,
                           "article" ,
                           "artist" ,
                           "artistic" ,
                           "Asian" ,
                           "aside" ,
                           "asleep" ,
                           "aspect" ,
                           "assault" ,
                           "assert" ,
                           "assess" ,
                           "assessment" ,
                           "asset" ,
                           "assign" ,
                           "assignment" ,
                           "assist" ,
                           "assistance" ,
                           "assistant" ,
                           "associate" ,
                           "association" ,
                           "assume" ,
                           "assumption" ,
                           "assure" ,
                           "athlete" ,
                           "athletic" ,
                           "atmosphere" ,
                           "attach" ,
                           "attack" ,
                           "attempt" ,
                           "attend" ,
                           "attention" ,
                           "attitude" ,
                           "attorney" ,
                           "attract" ,
                           "attractive" ,
                           "attribute" ,
                           "audience" ,
                           "author" ,
                           "authority" ,
                           "available" ,
                           "average" ,
                           "avoid" ,
                           "award" ,
                           "aware" ,
                           "awareness" ,
                           "awful" ,
                           "background" ,
                           "badly" ,
                           "balance" ,
                           "barely" ,
                           "barrel" ,
                           "barrier" ,
                           "baseball" ,
                           "basic" ,
                           "basically" ,
                           "basis" ,
                           "basket" ,
                           "basketball" ,
                           "bathroom" ,
                           "battery" ,
                           "battle" ,
                           "beach" ,
                           "beautiful" ,
                           "beauty" ,
                           "because" ,
                           "become" ,
                           "bedroom" ,
                           "before" ,
                           "begin" ,
                           "beginning" ,
                           "behavior" ,
                           "behind" ,
                           "being" ,
                           "belief" ,
                           "believe" ,
                           "belong" ,
                           "below" ,
                           "bench" ,
                           "beneath" ,
                           "benefit" ,
                           "beside" ,
                           "besides" ,
                           "better" ,
                           "between" ,
                           "beyond" ,
                           "Bible" ,
                           "billion" ,
                           "biological" ,
                           "birth" ,
                           "birthday" ,
                           "black" ,
                           "blade" ,
                           "blame" ,
                           "blanket" ,
                           "blind" ,
                           "block" ,
                           "blood" ,
                           "board" ,
                           "bombing" ,
                           "border" ,
                           "borrow" ,
                           "bother" ,
                           "bottle" ,
                           "bottom" ,
                           "boundary" ,
                           "boyfriend" ,
                           "brain" ,
                           "branch" ,
                           "brand" ,
                           "bread" ,
                           "break" ,
                           "breakfast" ,
                           "breast" ,
                           "breath" ,
                           "breathe" ,
                           "brick" ,
                           "bridge" ,
                           "brief" ,
                           "briefly" ,
                           "bright" ,
                           "brilliant" ,
                           "bring" ,
                           "British" ,
                           "broad" ,
                           "broken" ,
                           "brother" ,
                           "brown" ,
                           "brush" ,
                           "budget" ,
                           "build" ,
                           "building" ,
                           "bullet" ,
                           "bunch" ,
                           "burden" ,
                           "business" ,
                           "butter" ,
                           "button" ,
                           "buyer" ,
                           "cabin" ,
                           "cabinet" ,
                           "cable" ,
                           "calculate" ,
                           "camera" ,
                           "campaign" ,
                           "campus" ,
                           "Canadian" ,
                           "cancer" ,
                           "candidate" ,
                           "capability" ,
                           "capable" ,
                           "capacity" ,
                           "capital" ,
                           "captain" ,
                           "capture" ,
                           "carbon" ,
                           "career" ,
                           "careful" ,
                           "carefully" ,
                           "carrier" ,
                           "carry" ,
                           "catch" ,
                           "category" ,
                           "Catholic" ,
                           "cause" ,
                           "ceiling" ,
                           "celebrate" ,
                           "celebration" ,
                           "celebrity" ,
                           "center" ,
                           "central" ,
                           "century" ,
                           "ceremony" ,
                           "certain" ,
                           "certainly" ,
                           "chain" ,
                           "chair" ,
                           "chairman" ,
                           "challenge" ,
                           "chamber" ,
                           "champion" ,
                           "championship" ,
                           "chance" ,
                           "change" ,
                           "changing" ,
                           "channel" ,
                           "chapter" ,
                           "character" ,
                           "characteristic" ,
                           "characterize" ,
                           "charge" ,
                           "charity" ,
                           "chart" ,
                           "chase" ,
                           "cheap" ,
                           "check" ,
                           "cheek" ,
                           "cheese" ,
                           "chemical" ,
                           "chest" ,
                           "chicken" ,
                           "chief" ,
                           "child" ,
                           "childhood" ,
                           "Chinese" ,
                           "chocolate" ,
                           "choice" ,
                           "cholesterol" ,
                           "choose" ,
                           "Christian" ,
                           "Christmas" ,
                           "church" ,
                           "cigarette" ,
                           "circle" ,
                           "circumstance" ,
                           "citizen" ,
                           "civil" ,
                           "civilian" ,
                           "claim" ,
                           "class" ,
                           "classic" ,
                           "classroom" ,
                           "clean" ,
                           "clear" ,
                           "clearly" ,
                           "client" ,
                           "climate" ,
                           "climb" ,
                           "clinic" ,
                           "clinical" ,
                           "clock" ,
                           "close" ,
                           "closely" ,
                           "closer" ,
                           "clothes" ,
                           "clothing" ,
                           "cloud" ,
                           "cluster" ,
                           "coach" ,
                           "coalition" ,
                           "coast" ,
                           "coffee" ,
                           "cognitive" ,
                           "collapse" ,
                           "colleague" ,
                           "collect" ,
                           "collection" ,
                           "collective" ,
                           "college" ,
                           "colonial" ,
                           "color" ,
                           "column" ,
                           "combination" ,
                           "combine" ,
                           "comedy" ,
                           "comfort" ,
                           "comfortable" ,
                           "command" ,
                           "commander" ,
                           "comment" ,
                           "commercial" ,
                           "commission" ,
                           "commit" ,
                           "commitment" ,
                           "committee" ,
                           "common" ,
                           "communicate" ,
                           "communication" ,
                           "community" ,
                           "company" ,
                           "compare" ,
                           "comparison" ,
                           "compete" ,
                           "competition" ,
                           "competitive" ,
                           "competitor" ,
                           "complain" ,
                           "complaint" ,
                           "complete" ,
                           "completely" ,
                           "complex" ,
                           "complicated" ,
                           "component" ,
                           "compose" ,
                           "composition" ,
                           "comprehensive" ,
                           "computer" ,
                           "concentrate" ,
                           "concentration" ,
                           "concept" ,
                           "concern" ,
                           "concerned" ,
                           "concert" ,
                           "conclude" ,
                           "conclusion" ,
                           "concrete" ,
                           "condition" ,
                           "conduct" ,
                           "conference" ,
                           "confidence" ,
                           "confident" ,
                           "confirm" ,
                           "conflict" ,
                           "confront" ,
                           "confusion" ,
                           "Congress" ,
                           "congressional" ,
                           "connect" ,
                           "connection" ,
                           "consciousness" ,
                           "consensus" ,
                           "consequence" ,
                           "conservative" ,
                           "consider" ,
                           "considerable" ,
                           "consideration" ,
                           "consist" ,
                           "consistent" ,
                           "constant" ,
                           "constantly" ,
                           "constitute" ,
                           "constitutional" ,
                           "construct" ,
                           "construction" ,
                           "consultant" ,
                           "consume" ,
                           "consumer" ,
                           "consumption" ,
                           "contact" ,
                           "contain" ,
                           "container" ,
                           "contemporary" ,
                           "content" ,
                           "contest" ,
                           "context" ,
                           "continue" ,
                           "continued" ,
                           "contract" ,
                           "contrast" ,
                           "contribute" ,
                           "contribution" ,
                           "control" ,
                           "controversial" ,
                           "controversy" ,
                           "convention" ,
                           "conventional" ,
                           "conversation" ,
                           "convert" ,
                           "conviction" ,
                           "convince" ,
                           "cookie" ,
                           "cooking" ,
                           "cooperation" ,
                           "corner" ,
                           "corporate" ,
                           "corporation" ,
                           "correct" ,
                           "correspondent" ,
                           "cotton" ,
                           "couch" ,
                           "could" ,
                           "council" ,
                           "counselor" ,
                           "count" ,
                           "counter" ,
                           "country" ,
                           "county" ,
                           "couple" ,
                           "courage" ,
                           "course" ,
                           "court" ,
                           "cousin" ,
                           "cover" ,
                           "coverage" ,
                           "crack" ,
                           "craft" ,
                           "crash" ,
                           "crazy" ,
                           "cream" ,
                           "create" ,
                           "creation" ,
                           "creative" ,
                           "creature" ,
                           "credit" ,
                           "crime" ,
                           "criminal" ,
                           "crisis" ,
                           "criteria" ,
                           "critic" ,
                           "critical" ,
                           "criticism" ,
                           "criticize" ,
                           "cross" ,
                           "crowd" ,
                           "crucial" ,
                           "cultural" ,
                           "culture" ,
                           "curious" ,
                           "current" ,
                           "currently" ,
                           "curriculum" ,
                           "custom" ,
                           "customer" ,
                           "cycle" ,
                           "daily" ,
                           "damage" ,
                           "dance" ,
                           "danger" ,
                           "dangerous" ,
                           "darkness" ,
                           "daughter" ,
                           "dealer" ,
                           "death" ,
                           "debate" ,
                           "decade" ,
                           "decide" ,
                           "decision" ,
                           "declare" ,
                           "decline" ,
                           "decrease" ,
                           "deeply" ,
                           "defeat" ,
                           "defend" ,
                           "defendant" ,
                           "defense" ,
                           "defensive" ,
                           "deficit" ,
                           "define" ,
                           "definitely" ,
                           "definition" ,
                           "degree" ,
                           "delay" ,
                           "deliver" ,
                           "delivery" ,
                           "demand" ,
                           "democracy" ,
                           "Democrat" ,
                           "democratic" ,
                           "demonstrate" ,
                           "demonstration" ,
                           "department" ,
                           "depend" ,
                           "dependent" ,
                           "depending" ,
                           "depict" ,
                           "depression" ,
                           "depth" ,
                           "deputy" ,
                           "derive" ,
                           "describe" ,
                           "description" ,
                           "desert" ,
                           "deserve" ,
                           "design" ,
                           "designer" ,
                           "desire" ,
                           "desperate" ,
                           "despite" ,
                           "destroy" ,
                           "destruction" ,
                           "detail" ,
                           "detailed" ,
                           "detect" ,
                           "determine" ,
                           "develop" ,
                           "developing" ,
                           "development" ,
                           "device" ,
                           "devote" ,
                           "dialogue" ,
                           "differ" ,
                           "difference" ,
                           "different" ,
                           "differently" ,
                           "difficult" ,
                           "difficulty" ,
                           "digital" ,
                           "dimension" ,
                           "dining" ,
                           "dinner" ,
                           "direct" ,
                           "direction" ,
                           "directly" ,
                           "director" ,
                           "dirty" ,
                           "disability" ,
                           "disagree" ,
                           "disappear" ,
                           "disaster" ,
                           "discipline" ,
                           "discourse" ,
                           "discover" ,
                           "discovery" ,
                           "discrimination" ,
                           "discuss" ,
                           "discussion" ,
                           "disease" ,
                           "dismiss" ,
                           "disorder" ,
                           "display" ,
                           "dispute" ,
                           "distance" ,
                           "distant" ,
                           "distinct" ,
                           "distinction" ,
                           "distinguish" ,
                           "distribute" ,
                           "distribution" ,
                           "district" ,
                           "diverse" ,
                           "diversity" ,
                           "divide" ,
                           "division" ,
                           "divorce" ,
                           "doctor" ,
                           "document" ,
                           "domestic" ,
                           "dominant" ,
                           "dominate" ,
                           "double" ,
                           "doubt" ,
                           "downtown" ,
                           "dozen" ,
                           "draft" ,
                           "drama" ,
                           "dramatic" ,
                           "dramatically" ,
                           "drawing" ,
                           "dream" ,
                           "dress" ,
                           "drink" ,
                           "drive" ,
                           "driver" ,
                           "during" ,
                           "eager" ,
                           "early" ,
                           "earnings" ,
                           "earth" ,
                           "easily" ,
                           "eastern" ,
                           "economic" ,
                           "economics" ,
                           "economist" ,
                           "economy" ,
                           "edition" ,
                           "editor" ,
                           "educate" ,
                           "education" ,
                           "educational" ,
                           "educator" ,
                           "effect" ,
                           "effective" ,
                           "effectively" ,
                           "efficiency" ,
                           "efficient" ,
                           "effort" ,
                           "eight" ,
                           "either" ,
                           "elderly" ,
                           "elect" ,
                           "election" ,
                           "electric" ,
                           "electricity" ,
                           "electronic" ,
                           "element" ,
                           "elementary" ,
                           "eliminate" ,
                           "elite" ,
                           "elsewhere" ,
                           "e-mail" ,
                           "embrace" ,
                           "emerge" ,
                           "emergency" ,
                           "emission" ,
                           "emotion" ,
                           "emotional" ,
                           "emphasis" ,
                           "emphasize" ,
                           "employ" ,
                           "employee" ,
                           "employer" ,
                           "employment" ,
                           "empty" ,
                           "enable" ,
                           "encounter" ,
                           "encourage" ,
                           "enemy" ,
                           "energy" ,
                           "enforcement" ,
                           "engage" ,
                           "engine" ,
                           "engineer" ,
                           "engineering" ,
                           "English" ,
                           "enhance" ,
                           "enjoy" ,
                           "enormous" ,
                           "enough" ,
                           "ensure" ,
                           "enter" ,
                           "enterprise" ,
                           "entertainment" ,
                           "entire" ,
                           "entirely" ,
                           "entrance" ,
                           "entry" ,
                           "environment" ,
                           "environmental" ,
                           "episode" ,
                           "equal" ,
                           "equally" ,
                           "equipment" ,
                           "error" ,
                           "escape" ,
                           "especially" ,
                           "essay" ,
                           "essential" ,
                           "essentially" ,
                           "establish" ,
                           "establishment" ,
                           "estate" ,
                           "estimate" ,
                           "ethics" ,
                           "ethnic" ,
                           "European" ,
                           "evaluate" ,
                           "evaluation" ,
                           "evening" ,
                           "event" ,
                           "eventually" ,
                           "every" ,
                           "everybody" ,
                           "everyday" ,
                           "everyone" ,
                           "everything" ,
                           "everywhere" ,
                           "evidence" ,
                           "evolution" ,
                           "evolve" ,
                           "exact" ,
                           "exactly" ,
                           "examination" ,
                           "examine" ,
                           "example" ,
                           "exceed" ,
                           "excellent" ,
                           "except" ,
                           "exception" ,
                           "exchange" ,
                           "exciting" ,
                           "executive" ,
                           "exercise" ,
                           "exhibit" ,
                           "exhibition" ,
                           "exist" ,
                           "existence" ,
                           "existing" ,
                           "expand" ,
                           "expansion" ,
                           "expect" ,
                           "expectation" ,
                           "expense" ,
                           "expensive" ,
                           "experience" ,
                           "experiment" ,
                           "expert" ,
                           "explain" ,
                           "explanation" ,
                           "explode" ,
                           "explore" ,
                           "explosion" ,
                           "expose" ,
                           "exposure" ,
                           "express" ,
                           "expression" ,
                           "extend" ,
                           "extension" ,
                           "extensive" ,
                           "extent" ,
                           "external" ,
                           "extra" ,
                           "extraordinary" ,
                           "extreme" ,
                           "extremely" ,
                           "fabric" ,
                           "facility" ,
                           "factor" ,
                           "factory" ,
                           "faculty" ,
                           "failure" ,
                           "fairly" ,
                           "faith" ,
                           "false" ,
                           "familiar" ,
                           "family" ,
                           "famous" ,
                           "fantasy" ,
                           "farmer" ,
                           "fashion" ,
                           "father" ,
                           "fault" ,
                           "favor" ,
                           "favorite" ,
                           "feature" ,
                           "federal" ,
                           "feeling" ,
                           "fellow" ,
                           "female" ,
                           "fence" ,
                           "fewer" ,
                           "fiber" ,
                           "fiction" ,
                           "field" ,
                           "fifteen" ,
                           "fifth" ,
                           "fifty" ,
                           "fight" ,
                           "fighter" ,
                           "fighting" ,
                           "figure" ,
                           "final" ,
                           "finally" ,
                           "finance" ,
                           "financial" ,
                           "finding" ,
                           "finger" ,
                           "finish" ,
                           "first" ,
                           "fishing" ,
                           "fitness" ,
                           "flame" ,
                           "flavor" ,
                           "flesh" ,
                           "flight" ,
                           "float" ,
                           "floor" ,
                           "flower" ,
                           "focus" ,
                           "follow" ,
                           "following" ,
                           "football" ,
                           "force" ,
                           "foreign" ,
                           "forest" ,
                           "forever" ,
                           "forget" ,
                           "formal" ,
                           "formation" ,
                           "former" ,
                           "formula" ,
                           "forth" ,
                           "fortune" ,
                           "forward" ,
                           "found" ,
                           "foundation" ,
                           "founder" ,
                           "fourth" ,
                           "frame" ,
                           "framework" ,
                           "freedom" ,
                           "freeze" ,
                           "French" ,
                           "frequency" ,
                           "frequent" ,
                           "frequently" ,
                           "fresh" ,
                           "friend" ,
                           "friendly" ,
                           "friendship" ,
                           "front" ,
                           "fruit" ,
                           "frustration" ,
                           "fully" ,
                           "function" ,
                           "fundamental" ,
                           "funding" ,
                           "funeral" ,
                           "funny" ,
                           "furniture" ,
                           "furthermore" ,
                           "future" ,
                           "galaxy" ,
                           "gallery" ,
                           "garage" ,
                           "garden" ,
                           "garlic" ,
                           "gather" ,
                           "gender" ,
                           "general" ,
                           "generally" ,
                           "generate" ,
                           "generation" ,
                           "genetic" ,
                           "gentleman" ,
                           "gently" ,
                           "German" ,
                           "gesture" ,
                           "ghost" ,
                           "giant" ,
                           "gifted" ,
                           "girlfriend" ,
                           "given" ,
                           "glance" ,
                           "glass" ,
                           "global" ,
                           "glove" ,
                           "golden" ,
                           "government" ,
                           "governor" ,
                           "grade" ,
                           "gradually" ,
                           "graduate" ,
                           "grain" ,
                           "grand" ,
                           "grandfather" ,
                           "grandmother" ,
                           "grant" ,
                           "grass" ,
                           "grave" ,
                           "great" ,
                           "greatest" ,
                           "green" ,
                           "grocery" ,
                           "ground" ,
                           "group" ,
                           "growing" ,
                           "growth" ,
                           "guarantee" ,
                           "guard" ,
                           "guess" ,
                           "guest" ,
                           "guide" ,
                           "guideline" ,
                           "guilty" ,
                           "habit" ,
                           "habitat" ,
                           "handful" ,
                           "handle" ,
                           "happen" ,
                           "happy" ,
                           "hardly" ,
                           "headline" ,
                           "headquarters" ,
                           "health" ,
                           "healthy" ,
                           "hearing" ,
                           "heart" ,
                           "heaven" ,
                           "heavily" ,
                           "heavy" ,
                           "height" ,
                           "helicopter" ,
                           "hello" ,
                           "helpful" ,
                           "heritage" ,
                           "herself" ,
                           "highlight" ,
                           "highly" ,
                           "highway" ,
                           "himself" ,
                           "historian" ,
                           "historic" ,
                           "historical" ,
                           "history" ,
                           "holiday" ,
                           "homeless" ,
                           "honest" ,
                           "honey" ,
                           "honor" ,
                           "horizon" ,
                           "horror" ,
                           "horse" ,
                           "hospital" ,
                           "hotel" ,
                           "house" ,
                           "household" ,
                           "housing" ,
                           "however" ,
                           "human" ,
                           "humor" ,
                           "hundred" ,
                           "hungry" ,
                           "hunter" ,
                           "hunting" ,
                           "husband" ,
                           "hypothesis" ,
                           "ideal" ,
                           "identification" ,
                           "identify" ,
                           "identity" ,
                           "ignore" ,
                           "illegal" ,
                           "illness" ,
                           "illustrate" ,
                           "image" ,
                           "imagination" ,
                           "imagine" ,
                           "immediate" ,
                           "immediately" ,
                           "immigrant" ,
                           "immigration" ,
                           "impact" ,
                           "implement" ,
                           "implication" ,
                           "imply" ,
                           "importance" ,
                           "important" ,
                           "impose" ,
                           "impossible" ,
                           "impress" ,
                           "impression" ,
                           "impressive" ,
                           "improve" ,
                           "improvement" ,
                           "incentive" ,
                           "incident" ,
                           "include" ,
                           "including" ,
                           "income" ,
                           "incorporate" ,
                           "increase" ,
                           "increased" ,
                           "increasing" ,
                           "increasingly" ,
                           "incredible" ,
                           "indeed" ,
                           "independence" ,
                           "independent" ,
                           "index" ,
                           "Indian" ,
                           "indicate" ,
                           "indication" ,
                           "individual" ,
                           "industrial" ,
                           "industry" ,
                           "infant" ,
                           "infection" ,
                           "inflation" ,
                           "influence" ,
                           "inform" ,
                           "information" ,
                           "ingredient" ,
                           "initial" ,
                           "initially" ,
                           "initiative" ,
                           "injury" ,
                           "inner" ,
                           "innocent" ,
                           "inquiry" ,
                           "inside" ,
                           "insight" ,
                           "insist" ,
                           "inspire" ,
                           "install" ,
                           "instance" ,
                           "instead" ,
                           "institution" ,
                           "institutional" ,
                           "instruction" ,
                           "instructor" ,
                           "instrument" ,
                           "insurance" ,
                           "intellectual" ,
                           "intelligence" ,
                           "intend" ,
                           "intense" ,
                           "intensity" ,
                           "intention" ,
                           "interaction" ,
                           "interest" ,
                           "interested" ,
                           "interesting" ,
                           "internal" ,
                           "international" ,
                           "Internet" ,
                           "interpret" ,
                           "interpretation" ,
                           "intervention" ,
                           "interview" ,
                           "introduce" ,
                           "introduction" ,
                           "invasion" ,
                           "invest" ,
                           "investigate" ,
                           "investigation" ,
                           "investigator" ,
                           "investment" ,
                           "investor" ,
                           "invite" ,
                           "involve" ,
                           "involved" ,
                           "involvement" ,
                           "Iraqi" ,
                           "Irish" ,
                           "Islamic" ,
                           "island" ,
                           "Israeli" ,
                           "issue" ,
                           "Italian" ,
                           "itself" ,
                           "jacket" ,
                           "Japanese" ,
                           "Jewish" ,
                           "joint" ,
                           "journal" ,
                           "journalist" ,
                           "journey" ,
                           "judge" ,
                           "judgment" ,
                           "juice" ,
                           "junior" ,
                           "justice" ,
                           "justify" ,
                           "killer" ,
                           "killing" ,
                           "kitchen" ,
                           "knife" ,
                           "knock" ,
                           "knowledge" ,
                           "label" ,
                           "labor" ,
                           "laboratory" ,
                           "landscape" ,
                           "language" ,
                           "large" ,
                           "largely" ,
                           "later" ,
                           "Latin" ,
                           "latter" ,
                           "laugh" ,
                           "launch" ,
                           "lawsuit" ,
                           "lawyer" ,
                           "layer" ,
                           "leader" ,
                           "leadership" ,
                           "leading" ,
                           "league" ,
                           "learn" ,
                           "learning" ,
                           "least" ,
                           "leather" ,
                           "leave" ,
                           "legacy" ,
                           "legal" ,
                           "legend" ,
                           "legislation" ,
                           "legitimate" ,
                           "lemon" ,
                           "length" ,
                           "lesson" ,
                           "letter" ,
                           "level" ,
                           "liberal" ,
                           "library" ,
                           "license" ,
                           "lifestyle" ,
                           "lifetime" ,
                           "light" ,
                           "likely" ,
                           "limit" ,
                           "limitation" ,
                           "limited" ,
                           "listen" ,
                           "literally" ,
                           "literary" ,
                           "literature" ,
                           "little" ,
                           "living" ,
                           "local" ,
                           "locate" ,
                           "location" ,
                           "long-term" ,
                           "loose" ,
                           "lovely" ,
                           "lover" ,
                           "lower" ,
                           "lucky" ,
                           "lunch" ,
                           "machine" ,
                           "magazine" ,
                           "mainly" ,
                           "maintain" ,
                           "maintenance" ,
                           "major" ,
                           "majority" ,
                           "maker" ,
                           "makeup" ,
                           "manage" ,
                           "management" ,
                           "manager" ,
                           "manner" ,
                           "manufacturer" ,
                           "manufacturing" ,
                           "margin" ,
                           "market" ,
                           "marketing" ,
                           "marriage" ,
                           "married" ,
                           "marry" ,
                           "massive" ,
                           "master" ,
                           "match" ,
                           "material" ,
                           "matter" ,
                           "maybe" ,
                           "mayor" ,
                           "meaning" ,
                           "meanwhile" ,
                           "measure" ,
                           "measurement" ,
                           "mechanism" ,
                           "media" ,
                           "medical" ,
                           "medication" ,
                           "medicine" ,
                           "medium" ,
                           "meeting" ,
                           "member" ,
                           "membership" ,
                           "memory" ,
                           "mental" ,
                           "mention" ,
                           "merely" ,
                           "message" ,
                           "metal" ,
                           "meter" ,
                           "method" ,
                           "Mexican" ,
                           "middle" ,
                           "might" ,
                           "military" ,
                           "million" ,
                           "minister" ,
                           "minor" ,
                           "minority" ,
                           "minute" ,
                           "miracle" ,
                           "mirror" ,
                           "missile" ,
                           "mission" ,
                           "mistake" ,
                           "mixture" ,
                           "mm-hmm" ,
                           "model" ,
                           "moderate" ,
                           "modern" ,
                           "modest" ,
                           "moment" ,
                           "money" ,
                           "monitor" ,
                           "month" ,
                           "moral" ,
                           "moreover" ,
                           "morning" ,
                           "mortgage" ,
                           "mostly" ,
                           "mother" ,
                           "motion" ,
                           "motivation" ,
                           "motor" ,
                           "mount" ,
                           "mountain" ,
                           "mouse" ,
                           "mouth" ,
                           "movement" ,
                           "movie" ,
                           "multiple" ,
                           "murder" ,
                           "muscle" ,
                           "museum" ,
                           "music" ,
                           "musical" ,
                           "musician" ,
                           "Muslim" ,
                           "mutual" ,
                           "myself" ,
                           "mystery" ,
                           "naked" ,
                           "narrative" ,
                           "narrow" ,
                           "nation" ,
                           "national" ,
                           "native" ,
                           "natural" ,
                           "naturally" ,
                           "nature" ,
                           "nearby" ,
                           "nearly" ,
                           "necessarily" ,
                           "necessary" ,
                           "negative" ,
                           "negotiate" ,
                           "negotiation" ,
                           "neighbor" ,
                           "neighborhood" ,
                           "neither" ,
                           "nerve" ,
                           "nervous" ,
                           "network" ,
                           "never" ,
                           "nevertheless" ,
                           "newly" ,
                           "newspaper" ,
                           "night" ,
                           "nobody" ,
                           "noise" ,
                           "nomination" ,
                           "nonetheless" ,
                           "normal" ,
                           "normally" ,
                           "north" ,
                           "northern" ,
                           "nothing" ,
                           "notice" ,
                           "notion" ,
                           "novel" ,
                           "nowhere" ,
                           "nuclear" ,
                           "number" ,
                           "numerous" ,
                           "nurse" ,
                           "object" ,
                           "objective" ,
                           "obligation" ,
                           "observation" ,
                           "observe" ,
                           "observer" ,
                           "obtain" ,
                           "obvious" ,
                           "obviously" ,
                           "occasion" ,
                           "occasionally" ,
                           "occupation" ,
                           "occupy" ,
                           "occur" ,
                           "ocean" ,
                           "offense" ,
                           "offensive" ,
                           "offer" ,
                           "office" ,
                           "officer" ,
                           "official" ,
                           "often" ,
                           "Olympic" ,
                           "ongoing" ,
                           "onion" ,
                           "online" ,
                           "opening" ,
                           "operate" ,
                           "operating" ,
                           "operation" ,
                           "operator" ,
                           "opinion" ,
                           "opponent" ,
                           "opportunity" ,
                           "oppose" ,
                           "opposite" ,
                           "opposition" ,
                           "option" ,
                           "orange" ,
                           "order" ,
                           "ordinary" ,
                           "organic" ,
                           "organization" ,
                           "organize" ,
                           "orientation" ,
                           "origin" ,
                           "original" ,
                           "originally" ,
                           "other" ,
                           "others" ,
                           "otherwise" ,
                           "ought" ,
                           "ourselves" ,
                           "outcome" ,
                           "outside" ,
                           "overall" ,
                           "overcome" ,
                           "overlook" ,
                           "owner" ,
                           "package" ,
                           "painful" ,
                           "paint" ,
                           "painter" ,
                           "painting" ,
                           "Palestinian" ,
                           "panel" ,
                           "paper" ,
                           "parent" ,
                           "parking" ,
                           "participant" ,
                           "participate" ,
                           "participation" ,
                           "particular" ,
                           "particularly" ,
                           "partly" ,
                           "partner" ,
                           "partnership" ,
                           "party" ,
                           "passage" ,
                           "passenger" ,
                           "passion" ,
                           "patch" ,
                           "patient" ,
                           "pattern" ,
                           "pause" ,
                           "payment" ,
                           "peace" ,
                           "penalty" ,
                           "people" ,
                           "pepper" ,
                           "perceive" ,
                           "percentage" ,
                           "perception" ,
                           "perfect" ,
                           "perfectly" ,
                           "perform" ,
                           "performance" ,
                           "perhaps" ,
                           "period" ,
                           "permanent" ,
                           "permission" ,
                           "permit" ,
                           "person" ,
                           "personal" ,
                           "personality" ,
                           "personally" ,
                           "personnel" ,
                           "perspective" ,
                           "persuade" ,
                           "phase" ,
                           "phenomenon" ,
                           "philosophy" ,
                           "phone" ,
                           "photo" ,
                           "photograph" ,
                           "photographer" ,
                           "phrase" ,
                           "physical" ,
                           "physically" ,
                           "physician" ,
                           "piano" ,
                           "picture" ,
                           "piece" ,
                           "pilot" ,
                           "pitch" ,
                           "place" ,
                           "plane" ,
                           "planet" ,
                           "planning" ,
                           "plant" ,
                           "plastic" ,
                           "plate" ,
                           "platform" ,
                           "player" ,
                           "please" ,
                           "pleasure" ,
                           "plenty" ,
                           "pocket" ,
                           "poetry" ,
                           "point" ,
                           "police" ,
                           "policy" ,
                           "political" ,
                           "politically" ,
                           "politician" ,
                           "politics" ,
                           "pollution" ,
                           "popular" ,
                           "population" ,
                           "porch" ,
                           "portion" ,
                           "portrait" ,
                           "portray" ,
                           "position" ,
                           "positive" ,
                           "possess" ,
                           "possibility" ,
                           "possible" ,
                           "possibly" ,
                           "potato" ,
                           "potential" ,
                           "potentially" ,
                           "pound" ,
                           "poverty" ,
                           "powder" ,
                           "power" ,
                           "powerful" ,
                           "practical" ,
                           "practice" ,
                           "prayer" ,
                           "precisely" ,
                           "predict" ,
                           "prefer" ,
                           "preference" ,
                           "pregnancy" ,
                           "pregnant" ,
                           "preparation" ,
                           "prepare" ,
                           "prescription" ,
                           "presence" ,
                           "present" ,
                           "presentation" ,
                           "preserve" ,
                           "president" ,
                           "presidential" ,
                           "press" ,
                           "pressure" ,
                           "pretend" ,
                           "pretty" ,
                           "prevent" ,
                           "previous" ,
                           "previously" ,
                           "price" ,
                           "pride" ,
                           "priest" ,
                           "primarily" ,
                           "primary" ,
                           "prime" ,
                           "principal" ,
                           "principle" ,
                           "print" ,
                           "prior" ,
                           "priority" ,
                           "prison" ,
                           "prisoner" ,
                           "privacy" ,
                           "private" ,
                           "probably" ,
                           "problem" ,
                           "procedure" ,
                           "proceed" ,
                           "process" ,
                           "produce" ,
                           "producer" ,
                           "product" ,
                           "production" ,
                           "profession" ,
                           "professional" ,
                           "professor" ,
                           "profile" ,
                           "profit" ,
                           "program" ,
                           "progress" ,
                           "project" ,
                           "prominent" ,
                           "promise" ,
                           "promote" ,
                           "prompt" ,
                           "proof" ,
                           "proper" ,
                           "properly" ,
                           "property" ,
                           "proportion" ,
                           "proposal" ,
                           "propose" ,
                           "proposed" ,
                           "prosecutor" ,
                           "prospect" ,
                           "protect" ,
                           "protection" ,
                           "protein" ,
                           "protest" ,
                           "proud" ,
                           "prove" ,
                           "provide" ,
                           "provider" ,
                           "province" ,
                           "provision" ,
                           "psychological" ,
                           "psychologist" ,
                           "psychology" ,
                           "public" ,
                           "publication" ,
                           "publicly" ,
                           "publish" ,
                           "publisher" ,
                           "punishment" ,
                           "purchase" ,
                           "purpose" ,
                           "pursue" ,
                           "qualify" ,
                           "quality" ,
                           "quarter" ,
                           "quarterback" ,
                           "question" ,
                           "quick" ,
                           "quickly" ,
                           "quiet" ,
                           "quietly" ,
                           "quite" ,
                           "quote" ,
                           "racial" ,
                           "radical" ,
                           "radio" ,
                           "raise" ,
                           "range" ,
                           "rapid" ,
                           "rapidly" ,
                           "rarely" ,
                           "rather" ,
                           "rating" ,
                           "ratio" ,
                           "reach" ,
                           "react" ,
                           "reaction" ,
                           "reader" ,
                           "reading" ,
                           "ready" ,
                           "reality" ,
                           "realize" ,
                           "really" ,
                           "reason" ,
                           "reasonable" ,
                           "recall" ,
                           "receive" ,
                           "recent" ,
                           "recently" ,
                           "recipe" ,
                           "recognition" ,
                           "recognize" ,
                           "recommend" ,
                           "recommendation" ,
                           "record" ,
                           "recording" ,
                           "recover" ,
                           "recovery" ,
                           "recruit" ,
                           "reduce" ,
                           "reduction" ,
                           "refer" ,
                           "reference" ,
                           "reflect" ,
                           "reflection" ,
                           "reform" ,
                           "refugee" ,
                           "refuse" ,
                           "regard" ,
                           "regarding" ,
                           "regardless" ,
                           "regime" ,
                           "region" ,
                           "regional" ,
                           "register" ,
                           "regular" ,
                           "regularly" ,
                           "regulate" ,
                           "regulation" ,
                           "reinforce" ,
                           "reject" ,
                           "relate" ,
                           "relation" ,
                           "relationship" ,
                           "relative" ,
                           "relatively" ,
                           "relax" ,
                           "release" ,
                           "relevant" ,
                           "relief" ,
                           "religion" ,
                           "religious" ,
                           "remain" ,
                           "remaining" ,
                           "remarkable" ,
                           "remember" ,
                           "remind" ,
                           "remote" ,
                           "remove" ,
                           "repeat" ,
                           "repeatedly" ,
                           "replace" ,
                           "reply" ,
                           "report" ,
                           "reporter" ,
                           "represent" ,
                           "representation" ,
                           "representative" ,
                           "Republican" ,
                           "reputation" ,
                           "request" ,
                           "require" ,
                           "requirement" ,
                           "research" ,
                           "researcher" ,
                           "resemble" ,
                           "reservation" ,
                           "resident" ,
                           "resist" ,
                           "resistance" ,
                           "resolution" ,
                           "resolve" ,
                           "resort" ,
                           "resource" ,
                           "respect" ,
                           "respond" ,
                           "respondent" ,
                           "response" ,
                           "responsibility" ,
                           "responsible" ,
                           "restaurant" ,
                           "restore" ,
                           "restriction" ,
                           "result" ,
                           "retain" ,
                           "retire" ,
                           "retirement" ,
                           "return" ,
                           "reveal" ,
                           "revenue" ,
                           "review" ,
                           "revolution" ,
                           "rhythm" ,
                           "rifle" ,
                           "right" ,
                           "river" ,
                           "romantic" ,
                           "rough" ,
                           "roughly" ,
                           "round" ,
                           "route" ,
                           "routine" ,
                           "running" ,
                           "rural" ,
                           "Russian" ,
                           "sacred" ,
                           "safety" ,
                           "salad" ,
                           "salary" ,
                           "sales" ,
                           "sample" ,
                           "sanction" ,
                           "satellite" ,
                           "satisfaction" ,
                           "satisfy" ,
                           "sauce" ,
                           "saving" ,
                           "scale" ,
                           "scandal" ,
                           "scared" ,
                           "scenario" ,
                           "scene" ,
                           "schedule" ,
                           "scheme" ,
                           "scholar" ,
                           "scholarship" ,
                           "school" ,
                           "science" ,
                           "scientific" ,
                           "scientist" ,
                           "scope" ,
                           "score" ,
                           "scream" ,
                           "screen" ,
                           "script" ,
                           "search" ,
                           "season" ,
                           "second" ,
                           "secret" ,
                           "secretary" ,
                           "section" ,
                           "sector" ,
                           "secure" ,
                           "security" ,
                           "segment" ,
                           "seize" ,
                           "select" ,
                           "selection" ,
                           "Senate" ,
                           "senator" ,
                           "senior" ,
                           "sense" ,
                           "sensitive" ,
                           "sentence" ,
                           "separate" ,
                           "sequence" ,
                           "series" ,
                           "serious" ,
                           "seriously" ,
                           "serve" ,
                           "service" ,
                           "session" ,
                           "setting" ,
                           "settle" ,
                           "settlement" ,
                           "seven" ,
                           "several" ,
                           "severe" ,
                           "sexual" ,
                           "shade" ,
                           "shadow" ,
                           "shake" ,
                           "shall" ,
                           "shape" ,
                           "share" ,
                           "sharp" ,
                           "sheet" ,
                           "shelf" ,
                           "shell" ,
                           "shelter" ,
                           "shift" ,
                           "shine" ,
                           "shirt" ,
                           "shock" ,
                           "shoot" ,
                           "shooting" ,
                           "shopping" ,
                           "shore" ,
                           "short" ,
                           "shortly" ,
                           "should" ,
                           "shoulder" ,
                           "shout" ,
                           "shower" ,
                           "shrug" ,
                           "sight" ,
                           "signal" ,
                           "significance" ,
                           "significant" ,
                           "significantly" ,
                           "silence" ,
                           "silent" ,
                           "silver" ,
                           "similar" ,
                           "similarly" ,
                           "simple" ,
                           "simply" ,
                           "since" ,
                           "singer" ,
                           "single" ,
                           "sister" ,
                           "situation" ,
                           "skill" ,
                           "slave" ,
                           "sleep" ,
                           "slice" ,
                           "slide" ,
                           "slight" ,
                           "slightly" ,
                           "slowly" ,
                           "small" ,
                           "smart" ,
                           "smell" ,
                           "smile" ,
                           "smoke" ,
                           "smooth" ,
                           "so-called" ,
                           "soccer" ,
                           "social" ,
                           "society" ,
                           "software" ,
                           "solar" ,
                           "soldier" ,
                           "solid" ,
                           "solution" ,
                           "solve" ,
                           "somebody" ,
                           "somehow" ,
                           "someone" ,
                           "something" ,
                           "sometimes" ,
                           "somewhat" ,
                           "somewhere" ,
                           "sophisticated" ,
                           "sorry" ,
                           "sound" ,
                           "source" ,
                           "south" ,
                           "southern" ,
                           "Soviet" ,
                           "space" ,
                           "Spanish" ,
                           "speak" ,
                           "speaker" ,
                           "special" ,
                           "specialist" ,
                           "species" ,
                           "specific" ,
                           "specifically" ,
                           "speech" ,
                           "speed" ,
                           "spend" ,
                           "spending" ,
                           "spirit" ,
                           "spiritual" ,
                           "split" ,
                           "spokesman" ,
                           "sport" ,
                           "spread" ,
                           "spring" ,
                           "square" ,
                           "squeeze" ,
                           "stability" ,
                           "stable" ,
                           "staff" ,
                           "stage" ,
                           "stair" ,
                           "stake" ,
                           "stand" ,
                           "standard" ,
                           "standing" ,
                           "stare" ,
                           "start" ,
                           "state" ,
                           "statement" ,
                           "station" ,
                           "statistics" ,
                           "status" ,
                           "steady" ,
                           "steal" ,
                           "steel" ,
                           "stick" ,
                           "still" ,
                           "stock" ,
                           "stomach" ,
                           "stone" ,
                           "storage" ,
                           "store" ,
                           "storm" ,
                           "story" ,
                           "straight" ,
                           "strange" ,
                           "stranger" ,
                           "strategic" ,
                           "strategy" ,
                           "stream" ,
                           "street" ,
                           "strength" ,
                           "strengthen" ,
                           "stress" ,
                           "stretch" ,
                           "strike" ,
                           "string" ,
                           "strip" ,
                           "stroke" ,
                           "strong" ,
                           "strongly" ,
                           "structure" ,
                           "struggle" ,
                           "student" ,
                           "studio" ,
                           "study" ,
                           "stuff" ,
                           "stupid" ,
                           "style" ,
                           "subject" ,
                           "submit" ,
                           "subsequent" ,
                           "substance" ,
                           "substantial" ,
                           "succeed" ,
                           "success" ,
                           "successful" ,
                           "successfully" ,
                           "sudden" ,
                           "suddenly" ,
                           "suffer" ,
                           "sufficient" ,
                           "sugar" ,
                           "suggest" ,
                           "suggestion" ,
                           "suicide" ,
                           "summer" ,
                           "summit" ,
                           "super" ,
                           "supply" ,
                           "support" ,
                           "supporter" ,
                           "suppose" ,
                           "supposed" ,
                           "Supreme" ,
                           "surely" ,
                           "surface" ,
                           "surgery" ,
                           "surprise" ,
                           "surprised" ,
                           "surprising" ,
                           "surprisingly" ,
                           "surround" ,
                           "survey" ,
                           "survival" ,
                           "survive" ,
                           "survivor" ,
                           "suspect" ,
                           "sustain" ,
                           "swear" ,
                           "sweep" ,
                           "sweet" ,
                           "swing" ,
                           "switch" ,
                           "symbol" ,
                           "symptom" ,
                           "system" ,
                           "table" ,
                           "tablespoon" ,
                           "tactic" ,
                           "talent" ,
                           "target" ,
                           "taste" ,
                           "taxpayer" ,
                           "teach" ,
                           "teacher" ,
                           "teaching" ,
                           "teaspoon" ,
                           "technical" ,
                           "technique" ,
                           "technology" ,
                           "teenager" ,
                           "telephone" ,
                           "telescope" ,
                           "television" ,
                           "temperature" ,
                           "temporary" ,
                           "tendency" ,
                           "tennis" ,
                           "tension" ,
                           "terms" ,
                           "terrible" ,
                           "territory" ,
                           "terror" ,
                           "terrorism" ,
                           "terrorist" ,
                           "testify" ,
                           "testimony" ,
                           "testing" ,
                           "thank" ,
                           "thanks" ,
                           "theater" ,
                           "their" ,
                           "theme" ,
                           "themselves" ,
                           "theory" ,
                           "therapy" ,
                           "there" ,
                           "therefore" ,
                           "these" ,
                           "thick" ,
                           "thing" ,
                           "think" ,
                           "thinking" ,
                           "third" ,
                           "thirty" ,
                           "those" ,
                           "though" ,
                           "thought" ,
                           "thousand" ,
                           "threat" ,
                           "threaten" ,
                           "three" ,
                           "throat" ,
                           "through" ,
                           "throughout" ,
                           "throw" ,
                           "ticket" ,
                           "tight" ,
                           "tired" ,
                           "tissue" ,
                           "title" ,
                           "tobacco" ,
                           "today" ,
                           "together" ,
                           "tomato" ,
                           "tomorrow" ,
                           "tongue" ,
                           "tonight" ,
                           "tooth" ,
                           "topic" ,
                           "total" ,
                           "totally" ,
                           "touch" ,
                           "tough" ,
                           "tourist" ,
                           "tournament" ,
                           "toward" ,
                           "towards" ,
                           "tower" ,
                           "trace" ,
                           "track" ,
                           "trade" ,
                           "tradition" ,
                           "traditional" ,
                           "traffic" ,
                           "tragedy" ,
                           "trail" ,
                           "train" ,
                           "training" ,
                           "transfer" ,
                           "transform" ,
                           "transformation" ,
                           "transition" ,
                           "translate" ,
                           "transportation" ,
                           "travel" ,
                           "treat" ,
                           "treatment" ,
                           "treaty" ,
                           "tremendous" ,
                           "trend" ,
                           "trial" ,
                           "tribe" ,
                           "trick" ,
                           "troop" ,
                           "trouble" ,
                           "truck" ,
                           "truly" ,
                           "trust" ,
                           "truth" ,
                           "tunnel" ,
                           "twelve" ,
                           "twenty" ,
                           "twice" ,
                           "typical" ,
                           "typically" ,
                           "ultimate" ,
                           "ultimately" ,
                           "unable" ,
                           "uncle" ,
                           "under" ,
                           "undergo" ,
                           "understand" ,
                           "understanding" ,
                           "unfortunately" ,
                           "uniform" ,
                           "union" ,
                           "unique" ,
                           "United" ,
                           "universal" ,
                           "universe" ,
                           "university" ,
                           "unknown" ,
                           "unless" ,
                           "unlike" ,
                           "unlikely" ,
                           "until" ,
                           "unusual" ,
                           "upper" ,
                           "urban" ,
                           "useful" ,
                           "usual" ,
                           "usually" ,
                           "utility" ,
                           "vacation" ,
                           "valley" ,
                           "valuable" ,
                           "value" ,
                           "variable" ,
                           "variation" ,
                           "variety" ,
                           "various" ,
                           "vegetable" ,
                           "vehicle" ,
                           "venture" ,
                           "version" ,
                           "versus" ,
                           "vessel" ,
                           "veteran" ,
                           "victim" ,
                           "victory" ,
                           "video" ,
                           "viewer" ,
                           "village" ,
                           "violate" ,
                           "violation" ,
                           "violence" ,
                           "violent" ,
                           "virtually" ,
                           "virtue" ,
                           "virus" ,
                           "visible" ,
                           "vision" ,
                           "visit" ,
                           "visitor" ,
                           "visual" ,
                           "vital" ,
                           "voice" ,
                           "volume" ,
                           "volunteer" ,
                           "voter" ,
                           "vulnerable" ,
                           "wander" ,
                           "warning" ,
                           "waste" ,
                           "watch" ,
                           "water" ,
                           "wealth" ,
                           "wealthy" ,
                           "weapon" ,
                           "weather" ,
                           "wedding" ,
                           "weekend" ,
                           "weekly" ,
                           "weigh" ,
                           "weight" ,
                           "welcome" ,
                           "welfare" ,
                           "western" ,
                           "whatever" ,
                           "wheel" ,
                           "whenever" ,
                           "where" ,
                           "whereas" ,
                           "whether" ,
                           "which" ,
                           "while" ,
                           "whisper" ,
                           "white" ,
                           "whole" ,
                           "whose" ,
                           "widely" ,
                           "widespread" ,
                           "willing" ,
                           "window" ,
                           "winner" ,
                           "winter" ,
                           "wisdom" ,
                           "within" ,
                           "without" ,
                           "witness" ,
                           "woman" ,
                           "wonder" ,
                           "wonderful" ,
                           "wooden" ,
                           "worker" ,
                           "working" ,
                           "works" ,
                           "workshop" ,
                           "world" ,
                           "worried" ,
                           "worry" ,
                           "worth" ,
                           "would" ,
                           "wound" ,
                           "write" ,
                           "writer" ,
                           "writing" ,
                           "wrong" ,
                           "yellow" ,
                           "yesterday" ,
                           "yield" ,
                           "young" ,
                           "yours" ,
                           "yourself" ,
                           "youth",

};

     for(int i=0;i<2290;i++)
     {
         listofwords.push_back(words[i]);
     }

    rand:
    int random=rand()%listofwords.size();
    generatedword=words[random];
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
