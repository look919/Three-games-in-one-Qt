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
      addwordstolist();
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
             guessedletters.push_back(earLetters[random]);
             debugger();
             earLetters.erase(earLetters.begin()+random);

             k--;
         }
         while(computerscore==100)
         {
             delay(3);
             randomchar();
             setuiforpc(randchar);
             makewordlistshorter(randchar);
             conditionaloflistelements();
             debugger();
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
        guessedletters.push_back("e");
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

        while(computerscore==100)
         {
                 delay(3);
                 randomchar();
                 setuiforpc(randchar);
                 makewordlistshorter(randchar);
                 conditionaloflistelements();
                 debugger();          
        }
         endscreen();
    }           // End of normal, beginning of easy
    else
    {
        betteralphabetinvec();
        worsealphabetinvec();

        while(computerscore==100)
         {
                 delay(3);
                 randomchar();
                 setuiforpc(randchar);
                 makewordlistshorter(randchar);
                 conditionaloflistelements();
                 debugger();
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
    else if (listofwords.size()==1)
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
}
void Guessmyword::randomchar()
{
    index=0;
    if(listofwords.size()<=30 && level == 3)
    {
        returnNonRandomChar();
    }
    else if(listofwords.size()<=15 && level == 2)
    {
        returnNonRandomChar();
    }
    else if(listofwords.size()<=5 && level == 1)
    {
        returnNonRandomChar();
    }
    else{
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

}
bool Guessmyword::returnNonRandomChar()
{
    int word = 0;
    if(index>=4) {
        word++;
        index = 0;
    }

    bool letter = false;
    for(int i=0; i<guessedletters.size();i++){
       if(listofwords.at(word)[index] == guessedletters.at(i)) letter = true;
    }
    if(letter){
        index++;
        returnNonRandomChar();
    }
    else{
        randchar = listofwords.at(word)[index];
        guessedletters.push_back(listofwords.at(word)[index]);
        return true;
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
    qDebug()<<listofwords.size();
}

void Guessmyword::addwordstolist()
{
    srand((static_cast<unsigned int>(time(nullptr))));

    listofwords.push_back( "abandon" );         // should be 2287 words
    listofwords.push_back( "ability" );
    listofwords.push_back( "abortion" );
    listofwords.push_back( "about" );
    listofwords.push_back( "above" );
    listofwords.push_back( "abroad" );
    listofwords.push_back( "absence" );
    listofwords.push_back( "absolute" );
    listofwords.push_back( "absolutely" );
    listofwords.push_back( "absorb" );
    listofwords.push_back( "abuse" );
    listofwords.push_back( "academic" );
    listofwords.push_back( "accept" );
    listofwords.push_back( "access" );
    listofwords.push_back( "accident" );
    listofwords.push_back( "accompany" );
    listofwords.push_back( "accomplish" );
    listofwords.push_back( "according" );
    listofwords.push_back( "account" );
    listofwords.push_back( "accurate" );
    listofwords.push_back( "accuse" );
    listofwords.push_back( "achieve" );
    listofwords.push_back( "achievement" );
    listofwords.push_back( "acknowledge" );
    listofwords.push_back( "acquire" );
    listofwords.push_back( "across" );
    listofwords.push_back( "action" );
    listofwords.push_back( "active" );
    listofwords.push_back( "activist" );
    listofwords.push_back( "activity" );
    listofwords.push_back( "actor" );
    listofwords.push_back( "actress" );
    listofwords.push_back( "actual" );
    listofwords.push_back( "actually" );
    listofwords.push_back( "adapt" );
    listofwords.push_back( "addition" );
    listofwords.push_back( "additional" );
    listofwords.push_back( "address" );
    listofwords.push_back( "adequate" );
    listofwords.push_back( "adjust" );
    listofwords.push_back( "adjustment" );
    listofwords.push_back( "administration" );
    listofwords.push_back( "administrator" );
    listofwords.push_back( "admire" );
    listofwords.push_back( "admission" );
    listofwords.push_back( "admit" );
    listofwords.push_back( "adolescent" );
    listofwords.push_back( "adopt" );
    listofwords.push_back( "adult" );
    listofwords.push_back( "advance" );
    listofwords.push_back( "advanced" );
    listofwords.push_back( "advantage" );
    listofwords.push_back( "adventure" );
    listofwords.push_back( "advertising" );
    listofwords.push_back( "advice" );
    listofwords.push_back( "advise" );
    listofwords.push_back( "adviser" );
    listofwords.push_back( "advocate" );
    listofwords.push_back( "affair" );
    listofwords.push_back( "affect" );
    listofwords.push_back( "afford" );
    listofwords.push_back( "afraid" );
    listofwords.push_back( "african" );
    listofwords.push_back( "after" );
    listofwords.push_back( "afternoon" );
    listofwords.push_back( "again" );
    listofwords.push_back( "against" );
    listofwords.push_back( "agency" );
    listofwords.push_back( "agenda" );
    listofwords.push_back( "agent" );
    listofwords.push_back( "aggressive" );
    listofwords.push_back( "agree" );
    listofwords.push_back( "agreement" );
    listofwords.push_back( "agricultural" );
    listofwords.push_back( "ahead" );
    listofwords.push_back( "aircraft" );
    listofwords.push_back( "airline" );
    listofwords.push_back( "airport" );
    listofwords.push_back( "album" );
    listofwords.push_back( "alcohol" );
    listofwords.push_back( "alive" );
    listofwords.push_back( "alliance" );
    listofwords.push_back( "allow" );
    listofwords.push_back( "almost" );
    listofwords.push_back( "alone" );
    listofwords.push_back( "along" );
    listofwords.push_back( "already" );
    listofwords.push_back( "alter" );
    listofwords.push_back( "alternative" );
    listofwords.push_back( "although" );
    listofwords.push_back( "always" );
    listofwords.push_back( "amazing" );
    listofwords.push_back( "american" );
    listofwords.push_back( "among" );
    listofwords.push_back( "amount" );
    listofwords.push_back( "analysis" );
    listofwords.push_back( "analyst" );
    listofwords.push_back( "analyze" );
    listofwords.push_back( "ancient" );
    listofwords.push_back( "anger" );
    listofwords.push_back( "angle" );
    listofwords.push_back( "angry" );
    listofwords.push_back( "animal" );
    listofwords.push_back( "anniversary" );
    listofwords.push_back( "announce" );
    listofwords.push_back( "annual" );
    listofwords.push_back( "another" );
    listofwords.push_back( "answer" );
    listofwords.push_back( "anticipate" );
    listofwords.push_back( "anxiety" );
    listofwords.push_back( "anybody" );
    listofwords.push_back( "anymore" );
    listofwords.push_back( "anyone" );
    listofwords.push_back( "anything" );
    listofwords.push_back( "anyway" );
    listofwords.push_back( "anywhere" );
    listofwords.push_back( "apart" );
    listofwords.push_back( "apartment" );
    listofwords.push_back( "apparent" );
    listofwords.push_back( "apparently" );
    listofwords.push_back( "appeal" );
    listofwords.push_back( "appear" );
    listofwords.push_back( "appearance" );
    listofwords.push_back( "apple" );
    listofwords.push_back( "application" );
    listofwords.push_back( "apply" );
    listofwords.push_back( "appoint" );
    listofwords.push_back( "appointment" );
    listofwords.push_back( "appreciate" );
    listofwords.push_back( "approach" );
    listofwords.push_back( "appropriate" );
    listofwords.push_back( "approval" );
    listofwords.push_back( "approve" );
    listofwords.push_back( "approximately" );
    listofwords.push_back( "architect" );
    listofwords.push_back( "argue" );
    listofwords.push_back( "argument" );
    listofwords.push_back( "arise" );
    listofwords.push_back( "armed" );
    listofwords.push_back( "around" );
    listofwords.push_back( "arrange" );
    listofwords.push_back( "arrangement" );
    listofwords.push_back( "arrest" );
    listofwords.push_back( "arrival" );
    listofwords.push_back( "arrive" );
    listofwords.push_back( "article" );
    listofwords.push_back( "artist" );
    listofwords.push_back( "artistic" );
    listofwords.push_back( "asian" );
    listofwords.push_back( "aside" );
    listofwords.push_back( "asleep" );
    listofwords.push_back( "aspect" );
    listofwords.push_back( "assault" );
    listofwords.push_back( "assert" );
    listofwords.push_back( "assess" );
    listofwords.push_back( "assessment" );
    listofwords.push_back( "asset" );
    listofwords.push_back( "assign" );
    listofwords.push_back( "assignment" );
    listofwords.push_back( "assist" );
    listofwords.push_back( "assistance" );
    listofwords.push_back( "assistant" );
    listofwords.push_back( "associate" );
    listofwords.push_back( "association" );
    listofwords.push_back( "assume" );
    listofwords.push_back( "assumption" );
    listofwords.push_back( "assure" );
    listofwords.push_back( "athlete" );
    listofwords.push_back( "athletic" );
    listofwords.push_back( "atmosphere" );
    listofwords.push_back( "attach" );
    listofwords.push_back( "attack" );
    listofwords.push_back( "attempt" );
    listofwords.push_back( "attend" );
    listofwords.push_back( "attention" );
    listofwords.push_back( "attitude" );
    listofwords.push_back( "attorney" );
    listofwords.push_back( "attract" );
    listofwords.push_back( "attractive" );
    listofwords.push_back( "attribute" );
    listofwords.push_back( "audience" );
    listofwords.push_back( "author" );
    listofwords.push_back( "authority" );
    listofwords.push_back( "available" );
    listofwords.push_back( "average" );
    listofwords.push_back( "avoid" );
    listofwords.push_back( "award" );
    listofwords.push_back( "aware" );
    listofwords.push_back( "awareness" );
    listofwords.push_back( "awful" );
    listofwords.push_back( "background" );
    listofwords.push_back( "badly" );
    listofwords.push_back( "balance" );
    listofwords.push_back( "barely" );
    listofwords.push_back( "barrel" );
    listofwords.push_back( "barrier" );
    listofwords.push_back( "baseball" );
    listofwords.push_back( "basic" );
    listofwords.push_back( "basically" );
    listofwords.push_back( "basis" );
    listofwords.push_back( "basket" );
    listofwords.push_back( "basketball" );
    listofwords.push_back( "bathroom" );
    listofwords.push_back( "battery" );
    listofwords.push_back( "battle" );
    listofwords.push_back( "beach" );
    listofwords.push_back( "beautiful" );
    listofwords.push_back( "beauty" );
    listofwords.push_back( "because" );
    listofwords.push_back( "become" );
    listofwords.push_back( "bedroom" );
    listofwords.push_back( "before" );
    listofwords.push_back( "begin" );
    listofwords.push_back( "beginning" );
    listofwords.push_back( "behavior" );
    listofwords.push_back( "behind" );
    listofwords.push_back( "being" );
    listofwords.push_back( "belief" );
    listofwords.push_back( "believe" );
    listofwords.push_back( "belong" );
    listofwords.push_back( "below" );
    listofwords.push_back( "bench" );
    listofwords.push_back( "beneath" );
    listofwords.push_back( "benefit" );
    listofwords.push_back( "beside" );
    listofwords.push_back( "besides" );
    listofwords.push_back( "better" );
    listofwords.push_back( "between" );
    listofwords.push_back( "beyond" );
    listofwords.push_back( "bible" );
    listofwords.push_back( "billion" );
    listofwords.push_back( "biological" );
    listofwords.push_back( "birth" );
    listofwords.push_back( "birthday" );
    listofwords.push_back( "black" );
    listofwords.push_back( "blade" );
    listofwords.push_back( "blame" );
    listofwords.push_back( "blanket" );
    listofwords.push_back( "blind" );
    listofwords.push_back( "block" );
    listofwords.push_back( "blood" );
    listofwords.push_back( "board" );
    listofwords.push_back( "bombing" );
    listofwords.push_back( "border" );
    listofwords.push_back( "borrow" );
    listofwords.push_back( "bother" );
    listofwords.push_back( "bottle" );
    listofwords.push_back( "bottom" );
    listofwords.push_back( "boundary" );
    listofwords.push_back( "boyfriend" );
    listofwords.push_back( "brain" );
    listofwords.push_back( "branch" );
    listofwords.push_back( "brand" );
    listofwords.push_back( "bread" );
    listofwords.push_back( "break" );
    listofwords.push_back( "breakfast" );
    listofwords.push_back( "breast" );
    listofwords.push_back( "breath" );
    listofwords.push_back( "breathe" );
    listofwords.push_back( "brick" );
    listofwords.push_back( "bridge" );
    listofwords.push_back( "brief" );
    listofwords.push_back( "briefly" );
    listofwords.push_back( "bright" );
    listofwords.push_back( "brilliant" );
    listofwords.push_back( "bring" );
    listofwords.push_back( "british" );
    listofwords.push_back( "broad" );
    listofwords.push_back( "broken" );
    listofwords.push_back( "brother" );
    listofwords.push_back( "brown" );
    listofwords.push_back( "brush" );
    listofwords.push_back( "budget" );
    listofwords.push_back( "build" );
    listofwords.push_back( "building" );
    listofwords.push_back( "bullet" );
    listofwords.push_back( "bunch" );
    listofwords.push_back( "burden" );
    listofwords.push_back( "business" );
    listofwords.push_back( "butter" );
    listofwords.push_back( "button" );
    listofwords.push_back( "buyer" );
    listofwords.push_back( "cabin" );
    listofwords.push_back( "cabinet" );
    listofwords.push_back( "cable" );
    listofwords.push_back( "calculate" );
    listofwords.push_back( "camera" );
    listofwords.push_back( "campaign" );
    listofwords.push_back( "campus" );
    listofwords.push_back( "canadian" );
    listofwords.push_back( "cancer" );
    listofwords.push_back( "candidate" );
    listofwords.push_back( "capability" );
    listofwords.push_back( "capable" );
    listofwords.push_back( "capacity" );
    listofwords.push_back( "capital" );
    listofwords.push_back( "captain" );
    listofwords.push_back( "capture" );
    listofwords.push_back( "carbon" );
    listofwords.push_back( "career" );
    listofwords.push_back( "careful" );
    listofwords.push_back( "carefully" );
    listofwords.push_back( "carrier" );
    listofwords.push_back( "carry" );
    listofwords.push_back( "catch" );
    listofwords.push_back( "category" );
    listofwords.push_back( "catholic" );
    listofwords.push_back( "cause" );
    listofwords.push_back( "ceiling" );
    listofwords.push_back( "celebrate" );
    listofwords.push_back( "celebration" );
    listofwords.push_back( "celebrity" );
    listofwords.push_back( "center" );
    listofwords.push_back( "central" );
    listofwords.push_back( "century" );
    listofwords.push_back( "ceremony" );
    listofwords.push_back( "certain" );
    listofwords.push_back( "certainly" );
    listofwords.push_back( "chain" );
    listofwords.push_back( "chair" );
    listofwords.push_back( "chairman" );
    listofwords.push_back( "challenge" );
    listofwords.push_back( "chamber" );
    listofwords.push_back( "champion" );
    listofwords.push_back( "championship" );
    listofwords.push_back( "chance" );
    listofwords.push_back( "change" );
    listofwords.push_back( "changing" );
    listofwords.push_back( "channel" );
    listofwords.push_back( "chapter" );
    listofwords.push_back( "character" );
    listofwords.push_back( "characteristic" );
    listofwords.push_back( "characterize" );
    listofwords.push_back( "charge" );
    listofwords.push_back( "charity" );
    listofwords.push_back( "chart" );
    listofwords.push_back( "chase" );
    listofwords.push_back( "cheap" );
    listofwords.push_back( "check" );
    listofwords.push_back( "cheek" );
    listofwords.push_back( "cheese" );
    listofwords.push_back( "chemical" );
    listofwords.push_back( "chest" );
    listofwords.push_back( "chicken" );
    listofwords.push_back( "chief" );
    listofwords.push_back( "child" );
    listofwords.push_back( "childhood" );
    listofwords.push_back( "chinese" );
    listofwords.push_back( "chocolate" );
    listofwords.push_back( "choice" );
    listofwords.push_back( "cholesterol" );
    listofwords.push_back( "choose" );
    listofwords.push_back( "christian" );
    listofwords.push_back( "christmas" );
    listofwords.push_back( "church" );
    listofwords.push_back( "cigarette" );
    listofwords.push_back( "circle" );
    listofwords.push_back( "circumstance" );
    listofwords.push_back( "citizen" );
    listofwords.push_back( "civil" );
    listofwords.push_back( "civilian" );
    listofwords.push_back( "claim" );
    listofwords.push_back( "class" );
    listofwords.push_back( "classic" );
    listofwords.push_back( "classroom" );
    listofwords.push_back( "clean" );
    listofwords.push_back( "clear" );
    listofwords.push_back( "clearly" );
    listofwords.push_back( "client" );
    listofwords.push_back( "climate" );
    listofwords.push_back( "climb" );
    listofwords.push_back( "clinic" );
    listofwords.push_back( "clinical" );
    listofwords.push_back( "clock" );
    listofwords.push_back( "close" );
    listofwords.push_back( "closely" );
    listofwords.push_back( "closer" );
    listofwords.push_back( "clothes" );
    listofwords.push_back( "clothing" );
    listofwords.push_back( "cloud" );
    listofwords.push_back( "cluster" );
    listofwords.push_back( "coach" );
    listofwords.push_back( "coalition" );
    listofwords.push_back( "coast" );
    listofwords.push_back( "coffee" );
    listofwords.push_back( "cognitive" );
    listofwords.push_back( "collapse" );
    listofwords.push_back( "colleague" );
    listofwords.push_back( "collect" );
    listofwords.push_back( "collection" );
    listofwords.push_back( "collective" );
    listofwords.push_back( "college" );
    listofwords.push_back( "colonial" );
    listofwords.push_back( "color" );
    listofwords.push_back( "column" );
    listofwords.push_back( "combination" );
    listofwords.push_back( "combine" );
    listofwords.push_back( "comedy" );
    listofwords.push_back( "comfort" );
    listofwords.push_back( "comfortable" );
    listofwords.push_back( "command" );
    listofwords.push_back( "commander" );
    listofwords.push_back( "comment" );
    listofwords.push_back( "commercial" );
    listofwords.push_back( "commission" );
    listofwords.push_back( "commit" );
    listofwords.push_back( "commitment" );
    listofwords.push_back( "committee" );
    listofwords.push_back( "common" );
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
    listofwords.push_back( "cookie" );
    listofwords.push_back( "cooking" );
    listofwords.push_back( "cooperation" );
    listofwords.push_back( "corner" );
    listofwords.push_back( "corporate" );
    listofwords.push_back( "corporation" );
    listofwords.push_back( "correct" );
    listofwords.push_back( "correspondent" );
    listofwords.push_back( "cotton" );
    listofwords.push_back( "couch" );
    listofwords.push_back( "could" );
    listofwords.push_back( "council" );
    listofwords.push_back( "counselor" );
    listofwords.push_back( "count" );
    listofwords.push_back( "counter" );
    listofwords.push_back( "country" );
    listofwords.push_back( "county" );
    listofwords.push_back( "couple" );
    listofwords.push_back( "courage" );
    listofwords.push_back( "course" );
    listofwords.push_back( "court" );
    listofwords.push_back( "cousin" );
    listofwords.push_back( "cover" );
    listofwords.push_back( "coverage" );
    listofwords.push_back( "crack" );
    listofwords.push_back( "craft" );
    listofwords.push_back( "crash" );
    listofwords.push_back( "crazy" );
    listofwords.push_back( "cream" );
    listofwords.push_back( "create" );
    listofwords.push_back( "creation" );
    listofwords.push_back( "creative" );
    listofwords.push_back( "creature" );
    listofwords.push_back( "credit" );
    listofwords.push_back( "crime" );
    listofwords.push_back( "criminal" );
    listofwords.push_back( "crisis" );
    listofwords.push_back( "criteria" );
    listofwords.push_back( "critic" );
    listofwords.push_back( "critical" );
    listofwords.push_back( "criticism" );
    listofwords.push_back( "criticize" );
    listofwords.push_back( "cross" );
    listofwords.push_back( "crowd" );
    listofwords.push_back( "crucial" );
    listofwords.push_back( "cultural" );
    listofwords.push_back( "culture" );
    listofwords.push_back( "curious" );
    listofwords.push_back( "current" );
    listofwords.push_back( "currently" );
    listofwords.push_back( "curriculum" );
    listofwords.push_back( "custom" );
    listofwords.push_back( "customer" );
    listofwords.push_back( "cycle" );
    listofwords.push_back( "daily" );
    listofwords.push_back( "damage" );
    listofwords.push_back( "dance" );
    listofwords.push_back( "danger" );
    listofwords.push_back( "dangerous" );
    listofwords.push_back( "darkness" );
    listofwords.push_back( "daughter" );
    listofwords.push_back( "dealer" );
    listofwords.push_back( "death" );
    listofwords.push_back( "debate" );
    listofwords.push_back( "decade" );
    listofwords.push_back( "decide" );
    listofwords.push_back( "decision" );
    listofwords.push_back( "declare" );
    listofwords.push_back( "decline" );
    listofwords.push_back( "decrease" );
    listofwords.push_back( "deeply" );
    listofwords.push_back( "defeat" );
    listofwords.push_back( "defend" );
    listofwords.push_back( "defendant" );
    listofwords.push_back( "defense" );
    listofwords.push_back( "defensive" );
    listofwords.push_back( "deficit" );
    listofwords.push_back( "define" );
    listofwords.push_back( "definitely" );
    listofwords.push_back( "definition" );
    listofwords.push_back( "degree" );
    listofwords.push_back( "delay" );
    listofwords.push_back( "deliver" );
    listofwords.push_back( "delivery" );
    listofwords.push_back( "demand" );
    listofwords.push_back( "democracy" );
    listofwords.push_back( "democrat" );
    listofwords.push_back( "democratic" );
    listofwords.push_back( "demonstrate" );
    listofwords.push_back( "demonstration" );
    listofwords.push_back( "department" );
    listofwords.push_back( "depend" );
    listofwords.push_back( "dependent" );
    listofwords.push_back( "depending" );
    listofwords.push_back( "depict" );
    listofwords.push_back( "depression" );
    listofwords.push_back( "depth" );
    listofwords.push_back( "deputy" );
    listofwords.push_back( "derive" );
    listofwords.push_back( "describe" );
    listofwords.push_back( "description" );
    listofwords.push_back( "desert" );
    listofwords.push_back( "deserve" );
    listofwords.push_back( "design" );
    listofwords.push_back( "designer" );
    listofwords.push_back( "desire" );
    listofwords.push_back( "desperate" );
    listofwords.push_back( "despite" );
    listofwords.push_back( "destroy" );
    listofwords.push_back( "destruction" );
    listofwords.push_back( "detail" );
    listofwords.push_back( "detailed" );
    listofwords.push_back( "detect" );
    listofwords.push_back( "determine" );
    listofwords.push_back( "develop" );
    listofwords.push_back( "developing" );
    listofwords.push_back( "development" );
    listofwords.push_back( "device" );
    listofwords.push_back( "devote" );
    listofwords.push_back( "dialogue" );
    listofwords.push_back( "differ" );
    listofwords.push_back( "difference" );
    listofwords.push_back( "different" );
    listofwords.push_back( "differently" );
    listofwords.push_back( "difficult" );
    listofwords.push_back( "difficulty" );
    listofwords.push_back( "digital" );
    listofwords.push_back( "dimension" );
    listofwords.push_back( "dining" );
    listofwords.push_back( "dinner" );
    listofwords.push_back( "direct" );
    listofwords.push_back( "direction" );
    listofwords.push_back( "directly" );
    listofwords.push_back( "director" );
    listofwords.push_back( "dirty" );
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
    listofwords.push_back( "divide" );
    listofwords.push_back( "division" );
    listofwords.push_back( "divorce" );
    listofwords.push_back( "doctor" );
    listofwords.push_back( "document" );
    listofwords.push_back( "domestic" );
    listofwords.push_back( "dominant" );
    listofwords.push_back( "dominate" );
    listofwords.push_back( "double" );
    listofwords.push_back( "doubt" );
    listofwords.push_back( "downtown" );
    listofwords.push_back( "dozen" );
    listofwords.push_back( "draft" );
    listofwords.push_back( "drama" );
    listofwords.push_back( "dramatic" );
    listofwords.push_back( "dramatically" );
    listofwords.push_back( "drawing" );
    listofwords.push_back( "dream" );
    listofwords.push_back( "dress" );
    listofwords.push_back( "drink" );
    listofwords.push_back( "drive" );
    listofwords.push_back( "driver" );
    listofwords.push_back( "during" );
    listofwords.push_back( "eager" );
    listofwords.push_back( "early" );
    listofwords.push_back( "earnings" );
    listofwords.push_back( "earth" );
    listofwords.push_back( "easily" );
    listofwords.push_back( "eastern" );
    listofwords.push_back( "economic" );
    listofwords.push_back( "economics" );
    listofwords.push_back( "economist" );
    listofwords.push_back( "economy" );
    listofwords.push_back( "edition" );
    listofwords.push_back( "editor" );
    listofwords.push_back( "educate" );
    listofwords.push_back( "education" );
    listofwords.push_back( "educational" );
    listofwords.push_back( "educator" );
    listofwords.push_back( "effect" );
    listofwords.push_back( "effective" );
    listofwords.push_back( "effectively" );
    listofwords.push_back( "efficiency" );
    listofwords.push_back( "efficient" );
    listofwords.push_back( "effort" );
    listofwords.push_back( "eight" );
    listofwords.push_back( "either" );
    listofwords.push_back( "elderly" );
    listofwords.push_back( "elect" );
    listofwords.push_back( "election" );
    listofwords.push_back( "electric" );
    listofwords.push_back( "electricity" );
    listofwords.push_back( "electronic" );
    listofwords.push_back( "element" );
    listofwords.push_back( "elementary" );
    listofwords.push_back( "eliminate" );
    listofwords.push_back( "elite" );
    listofwords.push_back( "elsewhere" );
    listofwords.push_back( "e-mail" );
    listofwords.push_back( "embrace" );
    listofwords.push_back( "emerge" );
    listofwords.push_back( "emergency" );
    listofwords.push_back( "emission" );
    listofwords.push_back( "emotion" );
    listofwords.push_back( "emotional" );
    listofwords.push_back( "emphasis" );
    listofwords.push_back( "emphasize" );
    listofwords.push_back( "employ" );
    listofwords.push_back( "employee" );
    listofwords.push_back( "employer" );
    listofwords.push_back( "employment" );
    listofwords.push_back( "empty" );
    listofwords.push_back( "enable" );
    listofwords.push_back( "encounter" );
    listofwords.push_back( "encourage" );
    listofwords.push_back( "enemy" );
    listofwords.push_back( "energy" );
    listofwords.push_back( "enforcement" );
    listofwords.push_back( "engage" );
    listofwords.push_back( "engine" );
    listofwords.push_back( "engineer" );
    listofwords.push_back( "engineering" );
    listofwords.push_back( "english" );
    listofwords.push_back( "enhance" );
    listofwords.push_back( "enjoy" );
    listofwords.push_back( "enormous" );
    listofwords.push_back( "enough" );
    listofwords.push_back( "ensure" );
    listofwords.push_back( "enter" );
    listofwords.push_back( "enterprise" );
    listofwords.push_back( "entertainment" );
    listofwords.push_back( "entire" );
    listofwords.push_back( "entirely" );
    listofwords.push_back( "entrance" );
    listofwords.push_back( "entry" );
    listofwords.push_back( "environment" );
    listofwords.push_back( "environmental" );
    listofwords.push_back( "episode" );
    listofwords.push_back( "equal" );
    listofwords.push_back( "equally" );
    listofwords.push_back( "equipment" );
    listofwords.push_back( "error" );
    listofwords.push_back( "escape" );
    listofwords.push_back( "especially" );
    listofwords.push_back( "essay" );
    listofwords.push_back( "essential" );
    listofwords.push_back( "essentially" );
    listofwords.push_back( "establish" );
    listofwords.push_back( "establishment" );
    listofwords.push_back( "estate" );
    listofwords.push_back( "estimate" );
    listofwords.push_back( "ethics" );
    listofwords.push_back( "ethnic" );
    listofwords.push_back( "european" );
    listofwords.push_back( "evaluate" );
    listofwords.push_back( "evaluation" );
    listofwords.push_back( "evening" );
    listofwords.push_back( "event" );
    listofwords.push_back( "eventually" );
    listofwords.push_back( "every" );
    listofwords.push_back( "everybody" );
    listofwords.push_back( "everyday" );
    listofwords.push_back( "everyone" );
    listofwords.push_back( "everything" );
    listofwords.push_back( "everywhere" );
    listofwords.push_back( "evidence" );
    listofwords.push_back( "evolution" );
    listofwords.push_back( "evolve" );
    listofwords.push_back( "exact" );
    listofwords.push_back( "exactly" );
    listofwords.push_back( "examination" );
    listofwords.push_back( "examine" );
    listofwords.push_back( "example" );
    listofwords.push_back( "exceed" );
    listofwords.push_back( "excellent" );
    listofwords.push_back( "except" );
    listofwords.push_back( "exception" );
    listofwords.push_back( "exchange" );
    listofwords.push_back( "exciting" );
    listofwords.push_back( "executive" );
    listofwords.push_back( "exercise" );
    listofwords.push_back( "exhibit" );
    listofwords.push_back( "exhibition" );
    listofwords.push_back( "exist" );
    listofwords.push_back( "existence" );
    listofwords.push_back( "existing" );
    listofwords.push_back( "expand" );
    listofwords.push_back( "expansion" );
    listofwords.push_back( "expect" );
    listofwords.push_back( "expectation" );
    listofwords.push_back( "expense" );
    listofwords.push_back( "expensive" );
    listofwords.push_back( "experience" );
    listofwords.push_back( "experiment" );
    listofwords.push_back( "expert" );
    listofwords.push_back( "explain" );
    listofwords.push_back( "explanation" );
    listofwords.push_back( "explode" );
    listofwords.push_back( "explore" );
    listofwords.push_back( "explosion" );
    listofwords.push_back( "expose" );
    listofwords.push_back( "exposure" );
    listofwords.push_back( "express" );
    listofwords.push_back( "expression" );
    listofwords.push_back( "extend" );
    listofwords.push_back( "extension" );
    listofwords.push_back( "extensive" );
    listofwords.push_back( "extent" );
    listofwords.push_back( "external" );
    listofwords.push_back( "extra" );
    listofwords.push_back( "extraordinary" );
    listofwords.push_back( "extreme" );
    listofwords.push_back( "extremely" );
    listofwords.push_back( "fabric" );
    listofwords.push_back( "facility" );
    listofwords.push_back( "factor" );
    listofwords.push_back( "factory" );
    listofwords.push_back( "faculty" );
    listofwords.push_back( "failure" );
    listofwords.push_back( "fairly" );
    listofwords.push_back( "faith" );
    listofwords.push_back( "false" );
    listofwords.push_back( "familiar" );
    listofwords.push_back( "family" );
    listofwords.push_back( "famous" );
    listofwords.push_back( "fantasy" );
    listofwords.push_back( "farmer" );
    listofwords.push_back( "fashion" );
    listofwords.push_back( "father" );
    listofwords.push_back( "fault" );
    listofwords.push_back( "favor" );
    listofwords.push_back( "favorite" );
    listofwords.push_back( "feature" );
    listofwords.push_back( "federal" );
    listofwords.push_back( "feeling" );
    listofwords.push_back( "fellow" );
    listofwords.push_back( "female" );
    listofwords.push_back( "fence" );
    listofwords.push_back( "fewer" );
    listofwords.push_back( "fiber" );
    listofwords.push_back( "fiction" );
    listofwords.push_back( "field" );
    listofwords.push_back( "fifteen" );
    listofwords.push_back( "fifth" );
    listofwords.push_back( "fifty" );
    listofwords.push_back( "fight" );
    listofwords.push_back( "fighter" );
    listofwords.push_back( "fighting" );
    listofwords.push_back( "figure" );
    listofwords.push_back( "final" );
    listofwords.push_back( "finally" );
    listofwords.push_back( "finance" );
    listofwords.push_back( "financial" );
    listofwords.push_back( "finding" );
    listofwords.push_back( "finger" );
    listofwords.push_back( "finish" );
    listofwords.push_back( "first" );
    listofwords.push_back( "fishing" );
    listofwords.push_back( "fitness" );
    listofwords.push_back( "flame" );
    listofwords.push_back( "flavor" );
    listofwords.push_back( "flesh" );
    listofwords.push_back( "flight" );
    listofwords.push_back( "float" );
    listofwords.push_back( "floor" );
    listofwords.push_back( "flower" );
    listofwords.push_back( "focus" );
    listofwords.push_back( "follow" );
    listofwords.push_back( "following" );
    listofwords.push_back( "football" );
    listofwords.push_back( "force" );
    listofwords.push_back( "foreign" );
    listofwords.push_back( "forest" );
    listofwords.push_back( "forever" );
    listofwords.push_back( "forget" );
    listofwords.push_back( "formal" );
    listofwords.push_back( "formation" );
    listofwords.push_back( "former" );
    listofwords.push_back( "formula" );
    listofwords.push_back( "forth" );
    listofwords.push_back( "fortune" );
    listofwords.push_back( "forward" );
    listofwords.push_back( "found" );
    listofwords.push_back( "foundation" );
    listofwords.push_back( "founder" );
    listofwords.push_back( "fourth" );
    listofwords.push_back( "frame" );
    listofwords.push_back( "framework" );
    listofwords.push_back( "freedom" );
    listofwords.push_back( "freeze" );
    listofwords.push_back( "french" );
    listofwords.push_back( "frequency" );
    listofwords.push_back( "frequent" );
    listofwords.push_back( "frequently" );
    listofwords.push_back( "fresh" );
    listofwords.push_back( "friend" );
    listofwords.push_back( "friendly" );
    listofwords.push_back( "friendship" );
    listofwords.push_back( "front" );
    listofwords.push_back( "fruit" );
    listofwords.push_back( "frustration" );
    listofwords.push_back( "fully" );
    listofwords.push_back( "function" );
    listofwords.push_back( "fundamental" );
    listofwords.push_back( "funding" );
    listofwords.push_back( "funeral" );
    listofwords.push_back( "funny" );
    listofwords.push_back( "furniture" );
    listofwords.push_back( "furthermore" );
    listofwords.push_back( "future" );
    listofwords.push_back( "galaxy" );
    listofwords.push_back( "gallery" );
    listofwords.push_back( "garage" );
    listofwords.push_back( "garden" );
    listofwords.push_back( "garlic" );
    listofwords.push_back( "gather" );
    listofwords.push_back( "gender" );
    listofwords.push_back( "general" );
    listofwords.push_back( "generally" );
    listofwords.push_back( "generate" );
    listofwords.push_back( "generation" );
    listofwords.push_back( "genetic" );
    listofwords.push_back( "gentleman" );
    listofwords.push_back( "gently" );
    listofwords.push_back( "german" );
    listofwords.push_back( "gesture" );
    listofwords.push_back( "ghost" );
    listofwords.push_back( "giant" );
    listofwords.push_back( "gifted" );
    listofwords.push_back( "girlfriend" );
    listofwords.push_back( "given" );
    listofwords.push_back( "glance" );
    listofwords.push_back( "glass" );
    listofwords.push_back( "global" );
    listofwords.push_back( "glove" );
    listofwords.push_back( "golden" );
    listofwords.push_back( "government" );
    listofwords.push_back( "governor" );
    listofwords.push_back( "grade" );
    listofwords.push_back( "gradually" );
    listofwords.push_back( "graduate" );
    listofwords.push_back( "grain" );
    listofwords.push_back( "grand" );
    listofwords.push_back( "grandfather" );
    listofwords.push_back( "grandmother" );
    listofwords.push_back( "grant" );
    listofwords.push_back( "grass" );
    listofwords.push_back( "grave" );
    listofwords.push_back( "great" );
    listofwords.push_back( "greatest" );
    listofwords.push_back( "green" );
    listofwords.push_back( "grocery" );
    listofwords.push_back( "ground" );
    listofwords.push_back( "group" );
    listofwords.push_back( "growing" );
    listofwords.push_back( "growth" );
    listofwords.push_back( "guarantee" );
    listofwords.push_back( "guard" );
    listofwords.push_back( "guess" );
    listofwords.push_back( "guest" );
    listofwords.push_back( "guide" );
    listofwords.push_back( "guideline" );
    listofwords.push_back( "guilty" );
    listofwords.push_back( "habit" );
    listofwords.push_back( "habitat" );
    listofwords.push_back( "handful" );
    listofwords.push_back( "handle" );
    listofwords.push_back( "happen" );
    listofwords.push_back( "happy" );
    listofwords.push_back( "hardly" );
    listofwords.push_back( "headline" );
    listofwords.push_back( "headquarters" );
    listofwords.push_back( "health" );
    listofwords.push_back( "healthy" );
    listofwords.push_back( "hearing" );
    listofwords.push_back( "heart" );
    listofwords.push_back( "heaven" );
    listofwords.push_back( "heavily" );
    listofwords.push_back( "heavy" );
    listofwords.push_back( "height" );
    listofwords.push_back( "helicopter" );
    listofwords.push_back( "hello" );
    listofwords.push_back( "helpful" );
    listofwords.push_back( "heritage" );
    listofwords.push_back( "herself" );
    listofwords.push_back( "highlight" );
    listofwords.push_back( "highly" );
    listofwords.push_back( "highway" );
    listofwords.push_back( "himself" );
    listofwords.push_back( "historian" );
    listofwords.push_back( "historic" );
    listofwords.push_back( "historical" );
    listofwords.push_back( "history" );
    listofwords.push_back( "holiday" );
    listofwords.push_back( "homeless" );
    listofwords.push_back( "honest" );
    listofwords.push_back( "honey" );
    listofwords.push_back( "honor" );
    listofwords.push_back( "horizon" );
    listofwords.push_back( "horror" );
    listofwords.push_back( "horse" );
    listofwords.push_back( "hospital" );
    listofwords.push_back( "hotel" );
    listofwords.push_back( "house" );
    listofwords.push_back( "household" );
    listofwords.push_back( "housing" );
    listofwords.push_back( "however" );
    listofwords.push_back( "human" );
    listofwords.push_back( "humor" );
    listofwords.push_back( "hundred" );
    listofwords.push_back( "hungry" );
    listofwords.push_back( "hunter" );
    listofwords.push_back( "hunting" );
    listofwords.push_back( "husband" );
    listofwords.push_back( "hypothesis" );
    listofwords.push_back( "ideal" );
    listofwords.push_back( "identification" );
    listofwords.push_back( "identify" );
    listofwords.push_back( "identity" );
    listofwords.push_back( "ignore" );
    listofwords.push_back( "illegal" );
    listofwords.push_back( "illness" );
    listofwords.push_back( "illustrate" );
    listofwords.push_back( "image" );
    listofwords.push_back( "imagination" );
    listofwords.push_back( "imagine" );
    listofwords.push_back( "immediate" );
    listofwords.push_back( "immediately" );
    listofwords.push_back( "immigrant" );
    listofwords.push_back( "immigration" );
    listofwords.push_back( "impact" );
    listofwords.push_back( "implement" );
    listofwords.push_back( "implication" );
    listofwords.push_back( "imply" );
    listofwords.push_back( "importance" );
    listofwords.push_back( "important" );
    listofwords.push_back( "impose" );
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
    listofwords.push_back( "income" );
    listofwords.push_back( "incorporate" );
    listofwords.push_back( "increase" );
    listofwords.push_back( "increased" );
    listofwords.push_back( "increasing" );
    listofwords.push_back( "increasingly" );
    listofwords.push_back( "incredible" );
    listofwords.push_back( "indeed" );
    listofwords.push_back( "independence" );
    listofwords.push_back( "independent" );
    listofwords.push_back( "index" );
    listofwords.push_back( "indian" );
    listofwords.push_back( "indicate" );
    listofwords.push_back( "indication" );
    listofwords.push_back( "individual" );
    listofwords.push_back( "industrial" );
    listofwords.push_back( "industry" );
    listofwords.push_back( "infant" );
    listofwords.push_back( "infection" );
    listofwords.push_back( "inflation" );
    listofwords.push_back( "influence" );
    listofwords.push_back( "inform" );
    listofwords.push_back( "information" );
    listofwords.push_back( "ingredient" );
    listofwords.push_back( "initial" );
    listofwords.push_back( "initially" );
    listofwords.push_back( "initiative" );
    listofwords.push_back( "injury" );
    listofwords.push_back( "inner" );
    listofwords.push_back( "innocent" );
    listofwords.push_back( "inquiry" );
    listofwords.push_back( "inside" );
    listofwords.push_back( "insight" );
    listofwords.push_back( "insist" );
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
    listofwords.push_back( "intend" );
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
    listofwords.push_back( "invest" );
    listofwords.push_back( "investigate" );
    listofwords.push_back( "investigation" );
    listofwords.push_back( "investigator" );
    listofwords.push_back( "investment" );
    listofwords.push_back( "investor" );
    listofwords.push_back( "invite" );
    listofwords.push_back( "involve" );
    listofwords.push_back( "involved" );
    listofwords.push_back( "involvement" );
    listofwords.push_back( "iraqi" );
    listofwords.push_back( "irish" );
    listofwords.push_back( "islamic" );
    listofwords.push_back( "island" );
    listofwords.push_back( "israeli" );
    listofwords.push_back( "issue" );
    listofwords.push_back( "italian" );
    listofwords.push_back( "itself" );
    listofwords.push_back( "jacket" );
    listofwords.push_back( "japanese" );
    listofwords.push_back( "jewish" );
    listofwords.push_back( "joint" );
    listofwords.push_back( "journal" );
    listofwords.push_back( "journalist" );
    listofwords.push_back( "journey" );
    listofwords.push_back( "judge" );
    listofwords.push_back( "judgment" );
    listofwords.push_back( "juice" );
    listofwords.push_back( "junior" );
    listofwords.push_back( "justice" );
    listofwords.push_back( "justify" );
    listofwords.push_back( "killer" );
    listofwords.push_back( "killing" );
    listofwords.push_back( "kitchen" );
    listofwords.push_back( "knife" );
    listofwords.push_back( "knock" );
    listofwords.push_back( "knowledge" );
    listofwords.push_back( "label" );
    listofwords.push_back( "labor" );
    listofwords.push_back( "laboratory" );
    listofwords.push_back( "landscape" );
    listofwords.push_back( "language" );
    listofwords.push_back( "large" );
    listofwords.push_back( "largely" );
    listofwords.push_back( "later" );
    listofwords.push_back( "latin" );
    listofwords.push_back( "latter" );
    listofwords.push_back( "laugh" );
    listofwords.push_back( "launch" );
    listofwords.push_back( "lawsuit" );
    listofwords.push_back( "lawyer" );
    listofwords.push_back( "layer" );
    listofwords.push_back( "leader" );
    listofwords.push_back( "leadership" );
    listofwords.push_back( "leading" );
    listofwords.push_back( "league" );
    listofwords.push_back( "learn" );
    listofwords.push_back( "learning" );
    listofwords.push_back( "least" );
    listofwords.push_back( "leather" );
    listofwords.push_back( "leave" );
    listofwords.push_back( "legacy" );
    listofwords.push_back( "legal" );
    listofwords.push_back( "legend" );
    listofwords.push_back( "legislation" );
    listofwords.push_back( "legitimate" );
    listofwords.push_back( "lemon" );
    listofwords.push_back( "length" );
    listofwords.push_back( "lesson" );
    listofwords.push_back( "letter" );
    listofwords.push_back( "level" );
    listofwords.push_back( "liberal" );
    listofwords.push_back( "library" );
    listofwords.push_back( "license" );
    listofwords.push_back( "lifestyle" );
    listofwords.push_back( "lifetime" );
    listofwords.push_back( "light" );
    listofwords.push_back( "likely" );
    listofwords.push_back( "limit" );
    listofwords.push_back( "limitation" );
    listofwords.push_back( "limited" );
    listofwords.push_back( "listen" );
    listofwords.push_back( "literally" );
    listofwords.push_back( "literary" );
    listofwords.push_back( "literature" );
    listofwords.push_back( "little" );
    listofwords.push_back( "living" );
    listofwords.push_back( "local" );
    listofwords.push_back( "locate" );
    listofwords.push_back( "location" );
    listofwords.push_back( "long-term" );
    listofwords.push_back( "loose" );
    listofwords.push_back( "lovely" );
    listofwords.push_back( "lover" );
    listofwords.push_back( "lower" );
    listofwords.push_back( "lucky" );
    listofwords.push_back( "lunch" );
    listofwords.push_back( "machine" );
    listofwords.push_back( "magazine" );
    listofwords.push_back( "mainly" );
    listofwords.push_back( "maintain" );
    listofwords.push_back( "maintenance" );
    listofwords.push_back( "major" );
    listofwords.push_back( "majority" );
    listofwords.push_back( "maker" );
    listofwords.push_back( "makeup" );
    listofwords.push_back( "manage" );
    listofwords.push_back( "management" );
    listofwords.push_back( "manager" );
    listofwords.push_back( "manner" );
    listofwords.push_back( "manufacturer" );
    listofwords.push_back( "manufacturing" );
    listofwords.push_back( "margin" );
    listofwords.push_back( "market" );
    listofwords.push_back( "marketing" );
    listofwords.push_back( "marriage" );
    listofwords.push_back( "married" );
    listofwords.push_back( "marry" );
    listofwords.push_back( "massive" );
    listofwords.push_back( "master" );
    listofwords.push_back( "match" );
    listofwords.push_back( "material" );
    listofwords.push_back( "matter" );
    listofwords.push_back( "maybe" );
    listofwords.push_back( "mayor" );
    listofwords.push_back( "meaning" );
    listofwords.push_back( "meanwhile" );
    listofwords.push_back( "measure" );
    listofwords.push_back( "measurement" );
    listofwords.push_back( "mechanism" );
    listofwords.push_back( "media" );
    listofwords.push_back( "medical" );
    listofwords.push_back( "medication" );
    listofwords.push_back( "medicine" );
    listofwords.push_back( "medium" );
    listofwords.push_back( "meeting" );
    listofwords.push_back( "member" );
    listofwords.push_back( "membership" );
    listofwords.push_back( "memory" );
    listofwords.push_back( "mental" );
    listofwords.push_back( "mention" );
    listofwords.push_back( "merely" );
    listofwords.push_back( "message" );
    listofwords.push_back( "metal" );
    listofwords.push_back( "meter" );
    listofwords.push_back( "method" );
    listofwords.push_back( "mexican" );
    listofwords.push_back( "middle" );
    listofwords.push_back( "might" );
    listofwords.push_back( "military" );
    listofwords.push_back( "million" );
    listofwords.push_back( "minister" );
    listofwords.push_back( "minor" );
    listofwords.push_back( "minority" );
    listofwords.push_back( "minute" );
    listofwords.push_back( "miracle" );
    listofwords.push_back( "mirror" );
    listofwords.push_back( "missile" );
    listofwords.push_back( "mission" );
    listofwords.push_back( "mistake" );
    listofwords.push_back( "mixture" );
    listofwords.push_back( "mm-hmm" );
    listofwords.push_back( "model" );
    listofwords.push_back( "moderate" );
    listofwords.push_back( "modern" );
    listofwords.push_back( "modest" );
    listofwords.push_back( "moment" );
    listofwords.push_back( "money" );
    listofwords.push_back( "monitor" );
    listofwords.push_back( "month" );
    listofwords.push_back( "moral" );
    listofwords.push_back( "moreover" );
    listofwords.push_back( "morning" );
    listofwords.push_back( "mortgage" );
    listofwords.push_back( "mostly" );
    listofwords.push_back( "mother" );
    listofwords.push_back( "motion" );
    listofwords.push_back( "motivation" );
    listofwords.push_back( "motor" );
    listofwords.push_back( "mount" );
    listofwords.push_back( "mountain" );
    listofwords.push_back( "mouse" );
    listofwords.push_back( "mouth" );
    listofwords.push_back( "movement" );
    listofwords.push_back( "movie" );
    listofwords.push_back( "multiple" );
    listofwords.push_back( "murder" );
    listofwords.push_back( "muscle" );
    listofwords.push_back( "museum" );
    listofwords.push_back( "music" );
    listofwords.push_back( "musical" );
    listofwords.push_back( "musician" );
    listofwords.push_back( "muslim" );
    listofwords.push_back( "mutual" );
    listofwords.push_back( "myself" );
    listofwords.push_back( "mystery" );
    listofwords.push_back( "naked" );
    listofwords.push_back( "narrative" );
    listofwords.push_back( "narrow" );
    listofwords.push_back( "nation" );
    listofwords.push_back( "national" );
    listofwords.push_back( "native" );
    listofwords.push_back( "natural" );
    listofwords.push_back( "naturally" );
    listofwords.push_back( "nature" );
    listofwords.push_back( "nearby" );
    listofwords.push_back( "nearly" );
    listofwords.push_back( "necessarily" );
    listofwords.push_back( "necessary" );
    listofwords.push_back( "negative" );
    listofwords.push_back( "negotiate" );
    listofwords.push_back( "negotiation" );
    listofwords.push_back( "neighbor" );
    listofwords.push_back( "neighborhood" );
    listofwords.push_back( "neither" );
    listofwords.push_back( "nerve" );
    listofwords.push_back( "nervous" );
    listofwords.push_back( "network" );
    listofwords.push_back( "never" );
    listofwords.push_back( "nevertheless" );
    listofwords.push_back( "newly" );
    listofwords.push_back( "newspaper" );
    listofwords.push_back( "night" );
    listofwords.push_back( "nobody" );
    listofwords.push_back( "noise" );
    listofwords.push_back( "nomination" );
    listofwords.push_back( "nonetheless" );
    listofwords.push_back( "normal" );
    listofwords.push_back( "normally" );
    listofwords.push_back( "north" );
    listofwords.push_back( "northern" );
    listofwords.push_back( "nothing" );
    listofwords.push_back( "notice" );
    listofwords.push_back( "notion" );
    listofwords.push_back( "novel" );
    listofwords.push_back( "nowhere" );
    listofwords.push_back( "nuclear" );
    listofwords.push_back( "number" );
    listofwords.push_back( "numerous" );
    listofwords.push_back( "nurse" );
    listofwords.push_back( "object" );
    listofwords.push_back( "objective" );
    listofwords.push_back( "obligation" );
    listofwords.push_back( "observation" );
    listofwords.push_back( "observe" );
    listofwords.push_back( "observer" );
    listofwords.push_back( "obtain" );
    listofwords.push_back( "obvious" );
    listofwords.push_back( "obviously" );
    listofwords.push_back( "occasion" );
    listofwords.push_back( "occasionally" );
    listofwords.push_back( "occupation" );
    listofwords.push_back( "occupy" );
    listofwords.push_back( "occur" );
    listofwords.push_back( "ocean" );
    listofwords.push_back( "offense" );
    listofwords.push_back( "offensive" );
    listofwords.push_back( "offer" );
    listofwords.push_back( "office" );
    listofwords.push_back( "officer" );
    listofwords.push_back( "official" );
    listofwords.push_back( "often" );
    listofwords.push_back( "olympic" );
    listofwords.push_back( "ongoing" );
    listofwords.push_back( "onion" );
    listofwords.push_back( "online" );
    listofwords.push_back( "opening" );
    listofwords.push_back( "operate" );
    listofwords.push_back( "operating" );
    listofwords.push_back( "operation" );
    listofwords.push_back( "operator" );
    listofwords.push_back( "opinion" );
    listofwords.push_back( "opponent" );
    listofwords.push_back( "opportunity" );
    listofwords.push_back( "oppose" );
    listofwords.push_back( "opposite" );
    listofwords.push_back( "opposition" );
    listofwords.push_back( "option" );
    listofwords.push_back( "orange" );
    listofwords.push_back( "order" );
    listofwords.push_back( "ordinary" );
    listofwords.push_back( "organic" );
    listofwords.push_back( "organization" );
    listofwords.push_back( "organize" );
    listofwords.push_back( "orientation" );
    listofwords.push_back( "origin" );
    listofwords.push_back( "original" );
    listofwords.push_back( "originally" );
    listofwords.push_back( "other" );
    listofwords.push_back( "others" );
    listofwords.push_back( "otherwise" );
    listofwords.push_back( "ought" );
    listofwords.push_back( "ourselves" );
    listofwords.push_back( "outcome" );
    listofwords.push_back( "outside" );
    listofwords.push_back( "overall" );
    listofwords.push_back( "overcome" );
    listofwords.push_back( "overlook" );
    listofwords.push_back( "owner" );
    listofwords.push_back( "package" );
    listofwords.push_back( "painful" );
    listofwords.push_back( "paint" );
    listofwords.push_back( "painter" );
    listofwords.push_back( "painting" );
    listofwords.push_back( "palestinian" );
    listofwords.push_back( "panel" );
    listofwords.push_back( "paper" );
    listofwords.push_back( "parent" );
    listofwords.push_back( "parking" );
    listofwords.push_back( "participant" );
    listofwords.push_back( "participate" );
    listofwords.push_back( "participation" );
    listofwords.push_back( "particular" );
    listofwords.push_back( "particularly" );
    listofwords.push_back( "partly" );
    listofwords.push_back( "partner" );
    listofwords.push_back( "partnership" );
    listofwords.push_back( "party" );
    listofwords.push_back( "passage" );
    listofwords.push_back( "passenger" );
    listofwords.push_back( "passion" );
    listofwords.push_back( "patch" );
    listofwords.push_back( "patient" );
    listofwords.push_back( "pattern" );
    listofwords.push_back( "pause" );
    listofwords.push_back( "payment" );
    listofwords.push_back( "peace" );
    listofwords.push_back( "penalty" );
    listofwords.push_back( "people" );
    listofwords.push_back( "pepper" );
    listofwords.push_back( "perceive" );
    listofwords.push_back( "percentage" );
    listofwords.push_back( "perception" );
    listofwords.push_back( "perfect" );
    listofwords.push_back( "perfectly" );
    listofwords.push_back( "perform" );
    listofwords.push_back( "performance" );
    listofwords.push_back( "perhaps" );
    listofwords.push_back( "period" );
    listofwords.push_back( "permanent" );
    listofwords.push_back( "permission" );
    listofwords.push_back( "permit" );
    listofwords.push_back( "person" );
    listofwords.push_back( "personal" );
    listofwords.push_back( "personality" );
    listofwords.push_back( "personally" );
    listofwords.push_back( "personnel" );
    listofwords.push_back( "perspective" );
    listofwords.push_back( "persuade" );
    listofwords.push_back( "phase" );
    listofwords.push_back( "phenomenon" );
    listofwords.push_back( "philosophy" );
    listofwords.push_back( "phone" );
    listofwords.push_back( "photo" );
    listofwords.push_back( "photograph" );
    listofwords.push_back( "photographer" );
    listofwords.push_back( "phrase" );
    listofwords.push_back( "physical" );
    listofwords.push_back( "physically" );
    listofwords.push_back( "physician" );
    listofwords.push_back( "piano" );
    listofwords.push_back( "picture" );
    listofwords.push_back( "piece" );
    listofwords.push_back( "pilot" );
    listofwords.push_back( "pitch" );
    listofwords.push_back( "place" );
    listofwords.push_back( "plane" );
    listofwords.push_back( "planet" );
    listofwords.push_back( "planning" );
    listofwords.push_back( "plant" );
    listofwords.push_back( "plastic" );
    listofwords.push_back( "plate" );
    listofwords.push_back( "platform" );
    listofwords.push_back( "player" );
    listofwords.push_back( "please" );
    listofwords.push_back( "pleasure" );
    listofwords.push_back( "plenty" );
    listofwords.push_back( "pocket" );
    listofwords.push_back( "poetry" );
    listofwords.push_back( "point" );
    listofwords.push_back( "police" );
    listofwords.push_back( "policy" );
    listofwords.push_back( "political" );
    listofwords.push_back( "politically" );
    listofwords.push_back( "politician" );
    listofwords.push_back( "politics" );
    listofwords.push_back( "pollution" );
    listofwords.push_back( "popular" );
    listofwords.push_back( "population" );
    listofwords.push_back( "porch" );
    listofwords.push_back( "portion" );
    listofwords.push_back( "portrait" );
    listofwords.push_back( "portray" );
    listofwords.push_back( "position" );
    listofwords.push_back( "positive" );
    listofwords.push_back( "possess" );
    listofwords.push_back( "possibility" );
    listofwords.push_back( "possible" );
    listofwords.push_back( "possibly" );
    listofwords.push_back( "potato" );
    listofwords.push_back( "potential" );
    listofwords.push_back( "potentially" );
    listofwords.push_back( "pound" );
    listofwords.push_back( "poverty" );
    listofwords.push_back( "powder" );
    listofwords.push_back( "power" );
    listofwords.push_back( "powerful" );
    listofwords.push_back( "practical" );
    listofwords.push_back( "practice" );
    listofwords.push_back( "prayer" );
    listofwords.push_back( "precisely" );
    listofwords.push_back( "predict" );
    listofwords.push_back( "prefer" );
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
    listofwords.push_back( "press" );
    listofwords.push_back( "pressure" );
    listofwords.push_back( "pretend" );
    listofwords.push_back( "pretty" );
    listofwords.push_back( "prevent" );
    listofwords.push_back( "previous" );
    listofwords.push_back( "previously" );
    listofwords.push_back( "price" );
    listofwords.push_back( "pride" );
    listofwords.push_back( "priest" );
    listofwords.push_back( "primarily" );
    listofwords.push_back( "primary" );
    listofwords.push_back( "prime" );
    listofwords.push_back( "principal" );
    listofwords.push_back( "principle" );
    listofwords.push_back( "print" );
    listofwords.push_back( "prior" );
    listofwords.push_back( "priority" );
    listofwords.push_back( "prison" );
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
    listofwords.push_back( "profit" );
    listofwords.push_back( "program" );
    listofwords.push_back( "progress" );
    listofwords.push_back( "project" );
    listofwords.push_back( "prominent" );
    listofwords.push_back( "promise" );
    listofwords.push_back( "promote" );
    listofwords.push_back( "prompt" );
    listofwords.push_back( "proof" );
    listofwords.push_back( "proper" );
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
    listofwords.push_back( "proud" );
    listofwords.push_back( "prove" );
    listofwords.push_back( "provide" );
    listofwords.push_back( "provider" );
    listofwords.push_back( "province" );
    listofwords.push_back( "provision" );
    listofwords.push_back( "psychological" );
    listofwords.push_back( "psychologist" );
    listofwords.push_back( "psychology" );
    listofwords.push_back( "public" );
    listofwords.push_back( "publication" );
    listofwords.push_back( "publicly" );
    listofwords.push_back( "publish" );
    listofwords.push_back( "publisher" );
    listofwords.push_back( "punishment" );
    listofwords.push_back( "purchase" );
    listofwords.push_back( "purpose" );
    listofwords.push_back( "pursue" );
    listofwords.push_back( "qualify" );
    listofwords.push_back( "quality" );
    listofwords.push_back( "quarter" );
    listofwords.push_back( "quarterback" );
    listofwords.push_back( "question" );
    listofwords.push_back( "quick" );
    listofwords.push_back( "quickly" );
    listofwords.push_back( "quiet" );
    listofwords.push_back( "quietly" );
    listofwords.push_back( "quite" );
    listofwords.push_back( "quote" );
    listofwords.push_back( "racial" );
    listofwords.push_back( "radical" );
    listofwords.push_back( "radio" );
    listofwords.push_back( "raise" );
    listofwords.push_back( "range" );
    listofwords.push_back( "rapid" );
    listofwords.push_back( "rapidly" );
    listofwords.push_back( "rarely" );
    listofwords.push_back( "rather" );
    listofwords.push_back( "rating" );
    listofwords.push_back( "ratio" );
    listofwords.push_back( "reach" );
    listofwords.push_back( "react" );
    listofwords.push_back( "reaction" );
    listofwords.push_back( "reader" );
    listofwords.push_back( "reading" );
    listofwords.push_back( "ready" );
    listofwords.push_back( "reality" );
    listofwords.push_back( "realize" );
    listofwords.push_back( "really" );
    listofwords.push_back( "reason" );
    listofwords.push_back( "reasonable" );
    listofwords.push_back( "recall" );
    listofwords.push_back( "receive" );
    listofwords.push_back( "recent" );
    listofwords.push_back( "recently" );
    listofwords.push_back( "recipe" );
    listofwords.push_back( "recognition" );
    listofwords.push_back( "recognize" );
    listofwords.push_back( "recommend" );
    listofwords.push_back( "recommendation" );
    listofwords.push_back( "record" );
    listofwords.push_back( "recording" );
    listofwords.push_back( "recover" );
    listofwords.push_back( "recovery" );
    listofwords.push_back( "recruit" );
    listofwords.push_back( "reduce" );
    listofwords.push_back( "reduction" );
    listofwords.push_back( "refer" );
    listofwords.push_back( "reference" );
    listofwords.push_back( "reflect" );
    listofwords.push_back( "reflection" );
    listofwords.push_back( "reform" );
    listofwords.push_back( "refugee" );
    listofwords.push_back( "refuse" );
    listofwords.push_back( "regard" );
    listofwords.push_back( "regarding" );
    listofwords.push_back( "regardless" );
    listofwords.push_back( "regime" );
    listofwords.push_back( "region" );
    listofwords.push_back( "regional" );
    listofwords.push_back( "register" );
    listofwords.push_back( "regular" );
    listofwords.push_back( "regularly" );
    listofwords.push_back( "regulate" );
    listofwords.push_back( "regulation" );
    listofwords.push_back( "reinforce" );
    listofwords.push_back( "reject" );
    listofwords.push_back( "relate" );
    listofwords.push_back( "relation" );
    listofwords.push_back( "relationship" );
    listofwords.push_back( "relative" );
    listofwords.push_back( "relatively" );
    listofwords.push_back( "relax" );
    listofwords.push_back( "release" );
    listofwords.push_back( "relevant" );
    listofwords.push_back( "relief" );
    listofwords.push_back( "religion" );
    listofwords.push_back( "religious" );
    listofwords.push_back( "remain" );
    listofwords.push_back( "remaining" );
    listofwords.push_back( "remarkable" );
    listofwords.push_back( "remember" );
    listofwords.push_back( "remind" );
    listofwords.push_back( "remote" );
    listofwords.push_back( "remove" );
    listofwords.push_back( "repeat" );
    listofwords.push_back( "repeatedly" );
    listofwords.push_back( "replace" );
    listofwords.push_back( "reply" );
    listofwords.push_back( "report" );
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
    listofwords.push_back( "resist" );
    listofwords.push_back( "resistance" );
    listofwords.push_back( "resolution" );
    listofwords.push_back( "resolve" );
    listofwords.push_back( "resort" );
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
    listofwords.push_back( "result" );
    listofwords.push_back( "retain" );
    listofwords.push_back( "retire" );
    listofwords.push_back( "retirement" );
    listofwords.push_back( "return" );
    listofwords.push_back( "reveal" );
    listofwords.push_back( "revenue" );
    listofwords.push_back( "review" );
    listofwords.push_back( "revolution" );
    listofwords.push_back( "rhythm" );
    listofwords.push_back( "rifle" );
    listofwords.push_back( "right" );
    listofwords.push_back( "river" );
    listofwords.push_back( "romantic" );
    listofwords.push_back( "rough" );
    listofwords.push_back( "roughly" );
    listofwords.push_back( "round" );
    listofwords.push_back( "route" );
    listofwords.push_back( "routine" );
    listofwords.push_back( "running" );
    listofwords.push_back( "rural" );
    listofwords.push_back( "russian" );
    listofwords.push_back( "sacred" );
    listofwords.push_back( "safety" );
    listofwords.push_back( "salad" );
    listofwords.push_back( "salary" );
    listofwords.push_back( "sales" );
    listofwords.push_back( "sample" );
    listofwords.push_back( "sanction" );
    listofwords.push_back( "satellite" );
    listofwords.push_back( "satisfaction" );
    listofwords.push_back( "satisfy" );
    listofwords.push_back( "sauce" );
    listofwords.push_back( "saving" );
    listofwords.push_back( "scale" );
    listofwords.push_back( "scandal" );
    listofwords.push_back( "scared" );
    listofwords.push_back( "scenario" );
    listofwords.push_back( "scene" );
    listofwords.push_back( "schedule" );
    listofwords.push_back( "scheme" );
    listofwords.push_back( "scholar" );
    listofwords.push_back( "scholarship" );
    listofwords.push_back( "school" );
    listofwords.push_back( "science" );
    listofwords.push_back( "scientific" );
    listofwords.push_back( "scientist" );
    listofwords.push_back( "scope" );
    listofwords.push_back( "score" );
    listofwords.push_back( "scream" );
    listofwords.push_back( "screen" );
    listofwords.push_back( "script" );
    listofwords.push_back( "search" );
    listofwords.push_back( "season" );
    listofwords.push_back( "second" );
    listofwords.push_back( "secret" );
    listofwords.push_back( "secretary" );
    listofwords.push_back( "section" );
    listofwords.push_back( "sector" );
    listofwords.push_back( "secure" );
    listofwords.push_back( "security" );
    listofwords.push_back( "segment" );
    listofwords.push_back( "seize" );
    listofwords.push_back( "select" );
    listofwords.push_back( "selection" );
    listofwords.push_back( "senate" );
    listofwords.push_back( "senator" );
    listofwords.push_back( "senior" );
    listofwords.push_back( "sense" );
    listofwords.push_back( "sensitive" );
    listofwords.push_back( "sentence" );
    listofwords.push_back( "separate" );
    listofwords.push_back( "sequence" );
    listofwords.push_back( "series" );
    listofwords.push_back( "serious" );
    listofwords.push_back( "seriously" );
    listofwords.push_back( "serve" );
    listofwords.push_back( "service" );
    listofwords.push_back( "session" );
    listofwords.push_back( "setting" );
    listofwords.push_back( "settle" );
    listofwords.push_back( "settlement" );
    listofwords.push_back( "seven" );
    listofwords.push_back( "several" );
    listofwords.push_back( "severe" );
    listofwords.push_back( "sexual" );
    listofwords.push_back( "shade" );
    listofwords.push_back( "shadow" );
    listofwords.push_back( "shake" );
    listofwords.push_back( "shall" );
    listofwords.push_back( "shape" );
    listofwords.push_back( "share" );
    listofwords.push_back( "sharp" );
    listofwords.push_back( "sheet" );
    listofwords.push_back( "shelf" );
    listofwords.push_back( "shell" );
    listofwords.push_back( "shelter" );
    listofwords.push_back( "shift" );
    listofwords.push_back( "shine" );
    listofwords.push_back( "shirt" );
    listofwords.push_back( "shock" );
    listofwords.push_back( "shoot" );
    listofwords.push_back( "shooting" );
    listofwords.push_back( "shopping" );
    listofwords.push_back( "shore" );
    listofwords.push_back( "short" );
    listofwords.push_back( "shortly" );
    listofwords.push_back( "should" );
    listofwords.push_back( "shoulder" );
    listofwords.push_back( "shout" );
    listofwords.push_back( "shower" );
    listofwords.push_back( "shrug" );
    listofwords.push_back( "sight" );
    listofwords.push_back( "signal" );
    listofwords.push_back( "significance" );
    listofwords.push_back( "significant" );
    listofwords.push_back( "significantly" );
    listofwords.push_back( "silence" );
    listofwords.push_back( "silent" );
    listofwords.push_back( "silver" );
    listofwords.push_back( "similar" );
    listofwords.push_back( "similarly" );
    listofwords.push_back( "simple" );
    listofwords.push_back( "simply" );
    listofwords.push_back( "since" );
    listofwords.push_back( "singer" );
    listofwords.push_back( "single" );
    listofwords.push_back( "sister" );
    listofwords.push_back( "situation" );
    listofwords.push_back( "skill" );
    listofwords.push_back( "slave" );
    listofwords.push_back( "sleep" );
    listofwords.push_back( "slice" );
    listofwords.push_back( "slide" );
    listofwords.push_back( "slight" );
    listofwords.push_back( "slightly" );
    listofwords.push_back( "slowly" );
    listofwords.push_back( "small" );
    listofwords.push_back( "smart" );
    listofwords.push_back( "smell" );
    listofwords.push_back( "smile" );
    listofwords.push_back( "smoke" );
    listofwords.push_back( "smooth" );
    listofwords.push_back( "so-called" );
    listofwords.push_back( "soccer" );
    listofwords.push_back( "social" );
    listofwords.push_back( "society" );
    listofwords.push_back( "software" );
    listofwords.push_back( "solar" );
    listofwords.push_back( "soldier" );
    listofwords.push_back( "solid" );
    listofwords.push_back( "solution" );
    listofwords.push_back( "solve" );
    listofwords.push_back( "somebody" );
    listofwords.push_back( "somehow" );
    listofwords.push_back( "someone" );
    listofwords.push_back( "something" );
    listofwords.push_back( "sometimes" );
    listofwords.push_back( "somewhat" );
    listofwords.push_back( "somewhere" );
    listofwords.push_back( "sophisticated" );
    listofwords.push_back( "sorry" );
    listofwords.push_back( "sound" );
    listofwords.push_back( "source" );
    listofwords.push_back( "south" );
    listofwords.push_back( "southern" );
    listofwords.push_back( "soviet" );
    listofwords.push_back( "space" );
    listofwords.push_back( "spanish" );
    listofwords.push_back( "speak" );
    listofwords.push_back( "speaker" );
    listofwords.push_back( "special" );
    listofwords.push_back( "specialist" );
    listofwords.push_back( "species" );
    listofwords.push_back( "specific" );
    listofwords.push_back( "specifically" );
    listofwords.push_back( "speech" );
    listofwords.push_back( "speed" );
    listofwords.push_back( "spend" );
    listofwords.push_back( "spending" );
    listofwords.push_back( "spirit" );
    listofwords.push_back( "spiritual" );
    listofwords.push_back( "split" );
    listofwords.push_back( "spokesman" );
    listofwords.push_back( "sport" );
    listofwords.push_back( "spread" );
    listofwords.push_back( "spring" );
    listofwords.push_back( "square" );
    listofwords.push_back( "squeeze" );
    listofwords.push_back( "stability" );
    listofwords.push_back( "stable" );
    listofwords.push_back( "staff" );
    listofwords.push_back( "stage" );
    listofwords.push_back( "stair" );
    listofwords.push_back( "stake" );
    listofwords.push_back( "stand" );
    listofwords.push_back( "standard" );
    listofwords.push_back( "standing" );
    listofwords.push_back( "stare" );
    listofwords.push_back( "start" );
    listofwords.push_back( "state" );
    listofwords.push_back( "statement" );
    listofwords.push_back( "station" );
    listofwords.push_back( "statistics" );
    listofwords.push_back( "status" );
    listofwords.push_back( "steady" );
    listofwords.push_back( "steal" );
    listofwords.push_back( "steel" );
    listofwords.push_back( "stick" );
    listofwords.push_back( "still" );
    listofwords.push_back( "stock" );
    listofwords.push_back( "stomach" );
    listofwords.push_back( "stone" );
    listofwords.push_back( "storage" );
    listofwords.push_back( "store" );
    listofwords.push_back( "storm" );
    listofwords.push_back( "story" );
    listofwords.push_back( "straight" );
    listofwords.push_back( "strange" );
    listofwords.push_back( "stranger" );
    listofwords.push_back( "strategic" );
    listofwords.push_back( "strategy" );
    listofwords.push_back( "stream" );
    listofwords.push_back( "street" );
    listofwords.push_back( "strength" );
    listofwords.push_back( "strengthen" );
    listofwords.push_back( "stress" );
    listofwords.push_back( "stretch" );
    listofwords.push_back( "strike" );
    listofwords.push_back( "string" );
    listofwords.push_back( "strip" );
    listofwords.push_back( "stroke" );
    listofwords.push_back( "strong" );
    listofwords.push_back( "strongly" );
    listofwords.push_back( "structure" );
    listofwords.push_back( "struggle" );
    listofwords.push_back( "student" );
    listofwords.push_back( "studio" );
    listofwords.push_back( "study" );
    listofwords.push_back( "stuff" );
    listofwords.push_back( "stupid" );
    listofwords.push_back( "style" );
    listofwords.push_back( "subject" );
    listofwords.push_back( "submit" );
    listofwords.push_back( "subsequent" );
    listofwords.push_back( "substance" );
    listofwords.push_back( "substantial" );
    listofwords.push_back( "succeed" );
    listofwords.push_back( "success" );
    listofwords.push_back( "successful" );
    listofwords.push_back( "successfully" );
    listofwords.push_back( "sudden" );
    listofwords.push_back( "suddenly" );
    listofwords.push_back( "suffer" );
    listofwords.push_back( "sufficient" );
    listofwords.push_back( "sugar" );
    listofwords.push_back( "suggest" );
    listofwords.push_back( "suggestion" );
    listofwords.push_back( "suicide" );
    listofwords.push_back( "summer" );
    listofwords.push_back( "summit" );
    listofwords.push_back( "super" );
    listofwords.push_back( "supply" );
    listofwords.push_back( "support" );
    listofwords.push_back( "supporter" );
    listofwords.push_back( "suppose" );
    listofwords.push_back( "supposed" );
    listofwords.push_back( "supreme" );
    listofwords.push_back( "surely" );
    listofwords.push_back( "surface" );
    listofwords.push_back( "surgery" );
    listofwords.push_back( "surprise" );
    listofwords.push_back( "surprised" );
    listofwords.push_back( "surprising" );
    listofwords.push_back( "surprisingly" );
    listofwords.push_back( "surround" );
    listofwords.push_back( "survey" );
    listofwords.push_back( "survival" );
    listofwords.push_back( "survive" );
    listofwords.push_back( "survivor" );
    listofwords.push_back( "suspect" );
    listofwords.push_back( "sustain" );
    listofwords.push_back( "swear" );
    listofwords.push_back( "sweep" );
    listofwords.push_back( "sweet" );
    listofwords.push_back( "swing" );
    listofwords.push_back( "switch" );
    listofwords.push_back( "symbol" );
    listofwords.push_back( "symptom" );
    listofwords.push_back( "system" );
    listofwords.push_back( "table" );
    listofwords.push_back( "tablespoon" );
    listofwords.push_back( "tactic" );
    listofwords.push_back( "talent" );
    listofwords.push_back( "target" );
    listofwords.push_back( "taste" );
    listofwords.push_back( "taxpayer" );
    listofwords.push_back( "teach" );
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
    listofwords.push_back( "tennis" );
    listofwords.push_back( "tension" );
    listofwords.push_back( "terms" );
    listofwords.push_back( "terrible" );
    listofwords.push_back( "territory" );
    listofwords.push_back( "terror" );
    listofwords.push_back( "terrorism" );
    listofwords.push_back( "terrorist" );
    listofwords.push_back( "testify" );
    listofwords.push_back( "testimony" );
    listofwords.push_back( "testing" );
    listofwords.push_back( "thank" );
    listofwords.push_back( "thanks" );
    listofwords.push_back( "theater" );
    listofwords.push_back( "their" );
    listofwords.push_back( "theme" );
    listofwords.push_back( "themselves" );
    listofwords.push_back( "theory" );
    listofwords.push_back( "therapy" );
    listofwords.push_back( "there" );
    listofwords.push_back( "therefore" );
    listofwords.push_back( "these" );
    listofwords.push_back( "thick" );
    listofwords.push_back( "thing" );
    listofwords.push_back( "think" );
    listofwords.push_back( "thinking" );
    listofwords.push_back( "third" );
    listofwords.push_back( "thirty" );
    listofwords.push_back( "those" );
    listofwords.push_back( "though" );
    listofwords.push_back( "thought" );
    listofwords.push_back( "thousand" );
    listofwords.push_back( "threat" );
    listofwords.push_back( "threaten" );
    listofwords.push_back( "three" );
    listofwords.push_back( "throat" );
    listofwords.push_back( "through" );
    listofwords.push_back( "throughout" );
    listofwords.push_back( "throw" );
    listofwords.push_back( "ticket" );
    listofwords.push_back( "tight" );
    listofwords.push_back( "tired" );
    listofwords.push_back( "tissue" );
    listofwords.push_back( "title" );
    listofwords.push_back( "tobacco" );
    listofwords.push_back( "today" );
    listofwords.push_back( "together" );
    listofwords.push_back( "tomato" );
    listofwords.push_back( "tomorrow" );
    listofwords.push_back( "tongue" );
    listofwords.push_back( "tonight" );
    listofwords.push_back( "tooth" );
    listofwords.push_back( "topic" );
    listofwords.push_back( "total" );
    listofwords.push_back( "totally" );
    listofwords.push_back( "touch" );
    listofwords.push_back( "tough" );
    listofwords.push_back( "tourist" );
    listofwords.push_back( "tournament" );
    listofwords.push_back( "toward" );
    listofwords.push_back( "towards" );
    listofwords.push_back( "tower" );
    listofwords.push_back( "trace" );
    listofwords.push_back( "track" );
    listofwords.push_back( "trade" );
    listofwords.push_back( "tradition" );
    listofwords.push_back( "traditional" );
    listofwords.push_back( "traffic" );
    listofwords.push_back( "tragedy" );
    listofwords.push_back( "trail" );
    listofwords.push_back( "train" );
    listofwords.push_back( "training" );
    listofwords.push_back( "transfer" );
    listofwords.push_back( "transform" );
    listofwords.push_back( "transformation" );
    listofwords.push_back( "transition" );
    listofwords.push_back( "translate" );
    listofwords.push_back( "transportation" );
    listofwords.push_back( "travel" );
    listofwords.push_back( "treat" );
    listofwords.push_back( "treatment" );
    listofwords.push_back( "treaty" );
    listofwords.push_back( "tremendous" );
    listofwords.push_back( "trend" );
    listofwords.push_back( "trial" );
    listofwords.push_back( "tribe" );
    listofwords.push_back( "trick" );
    listofwords.push_back( "troop" );
    listofwords.push_back( "trouble" );
    listofwords.push_back( "truck" );
    listofwords.push_back( "truly" );
    listofwords.push_back( "trust" );
    listofwords.push_back( "truth" );
    listofwords.push_back( "tunnel" );
    listofwords.push_back( "twelve" );
    listofwords.push_back( "twenty" );
    listofwords.push_back( "twice" );
    listofwords.push_back( "typical" );
    listofwords.push_back( "typically" );
    listofwords.push_back( "ultimate" );
    listofwords.push_back( "ultimately" );
    listofwords.push_back( "unable" );
    listofwords.push_back( "uncle" );
    listofwords.push_back( "under" );
    listofwords.push_back( "undergo" );
    listofwords.push_back( "understand" );
    listofwords.push_back( "understanding" );
    listofwords.push_back( "unfortunately" );
    listofwords.push_back( "uniform" );
    listofwords.push_back( "union" );
    listofwords.push_back( "unique" );
    listofwords.push_back( "united" );
    listofwords.push_back( "universal" );
    listofwords.push_back( "universe" );
    listofwords.push_back( "university" );
    listofwords.push_back( "unknown" );
    listofwords.push_back( "unless" );
    listofwords.push_back( "unlike" );
    listofwords.push_back( "unlikely" );
    listofwords.push_back( "until" );
    listofwords.push_back( "unusual" );
    listofwords.push_back( "upper" );
    listofwords.push_back( "urban" );
    listofwords.push_back( "useful" );
    listofwords.push_back( "usual" );
    listofwords.push_back( "usually" );
    listofwords.push_back( "utility" );
    listofwords.push_back( "vacation" );
    listofwords.push_back( "valley" );
    listofwords.push_back( "valuable" );
    listofwords.push_back( "value" );
    listofwords.push_back( "variable" );
    listofwords.push_back( "variation" );
    listofwords.push_back( "variety" );
    listofwords.push_back( "various" );
    listofwords.push_back( "vegetable" );
    listofwords.push_back( "vehicle" );
    listofwords.push_back( "venture" );
    listofwords.push_back( "version" );
    listofwords.push_back( "versus" );
    listofwords.push_back( "vessel" );
    listofwords.push_back( "veteran" );
    listofwords.push_back( "victim" );
    listofwords.push_back( "victory" );
    listofwords.push_back( "video" );
    listofwords.push_back( "viewer" );
    listofwords.push_back( "village" );
    listofwords.push_back( "violate" );
    listofwords.push_back( "violation" );
    listofwords.push_back( "violence" );
    listofwords.push_back( "violent" );
    listofwords.push_back( "virtually" );
    listofwords.push_back( "virtue" );
    listofwords.push_back( "virus" );
    listofwords.push_back( "visible" );
    listofwords.push_back( "vision" );
    listofwords.push_back( "visit" );
    listofwords.push_back( "visitor" );
    listofwords.push_back( "visual" );
    listofwords.push_back( "vital" );
    listofwords.push_back( "voice" );
    listofwords.push_back( "volume" );
    listofwords.push_back( "volunteer" );
    listofwords.push_back( "voter" );
    listofwords.push_back( "vulnerable" );
    listofwords.push_back( "wander" );
    listofwords.push_back( "warning" );
    listofwords.push_back( "waste" );
    listofwords.push_back( "watch" );
    listofwords.push_back( "water" );
    listofwords.push_back( "wealth" );
    listofwords.push_back( "wealthy" );
    listofwords.push_back( "weapon" );
    listofwords.push_back( "weather" );
    listofwords.push_back( "wedding" );
    listofwords.push_back( "weekend" );
    listofwords.push_back( "weekly" );
    listofwords.push_back( "weigh" );
    listofwords.push_back( "weight" );
    listofwords.push_back( "welcome" );
    listofwords.push_back( "welfare" );
    listofwords.push_back( "western" );
    listofwords.push_back( "whatever" );
    listofwords.push_back( "wheel" );
    listofwords.push_back( "whenever" );
    listofwords.push_back( "where" );
    listofwords.push_back( "whereas" );
    listofwords.push_back( "whether" );
    listofwords.push_back( "which" );
    listofwords.push_back( "while" );
    listofwords.push_back( "whisper" );
    listofwords.push_back( "white" );
    listofwords.push_back( "whole" );
    listofwords.push_back( "whose" );
    listofwords.push_back( "widely" );
    listofwords.push_back( "widespread" );
    listofwords.push_back( "willing" );
    listofwords.push_back( "window" );
    listofwords.push_back( "winner" );
    listofwords.push_back( "winter" );
    listofwords.push_back( "wisdom" );
    listofwords.push_back( "withdraw" );
    listofwords.push_back( "within" );
    listofwords.push_back( "without" );
    listofwords.push_back( "witness" );
    listofwords.push_back( "woman" );
    listofwords.push_back( "wonder" );
    listofwords.push_back( "wonderful" );
    listofwords.push_back( "wooden" );
    listofwords.push_back( "worker" );
    listofwords.push_back( "working" );
    listofwords.push_back( "works" );
    listofwords.push_back( "workshop" );
    listofwords.push_back( "world" );
    listofwords.push_back( "worried" );
    listofwords.push_back( "worry" );
    listofwords.push_back( "worth" );
    listofwords.push_back( "would" );
    listofwords.push_back( "wound" );
    listofwords.push_back( "write" );
    listofwords.push_back( "writer" );
    listofwords.push_back( "writing" );
    listofwords.push_back( "wrong" );
    listofwords.push_back( "yellow" );
    listofwords.push_back( "yesterday" );
    listofwords.push_back( "yield" );
    listofwords.push_back( "young" );
    listofwords.push_back( "yours" );
    listofwords.push_back( "yourself" );
    listofwords.push_back( "youth" );


    rand:
    int random=rand()%listofwords.size();
    generatedword=listofwords[random];
    wordlength=generatedword.length();

    if(level==3)
    {
       if((wordlength<5) || (wordlength>12)) goto rand;
    }
    else if(level==2)
    {
       if(wordlength<6 || wordlength>12) goto rand;
    }
    else if(level==1)
    {
       if((wordlength<7) || (wordlength>12)) goto rand;
    }


}


Guessmyword::~Guessmyword()
{
    delete ui;
}
