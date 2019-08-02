#ifndef GUESSMYWORD_H
#define GUESSMYWORD_H

#include <QDialog>

namespace Ui {
class Guessmyword;
}

class Guessmyword : public QDialog
{
    Q_OBJECT

public:
    explicit Guessmyword(QWidget *parent = nullptr);
    ~Guessmyword();

private slots:
    void on_startquitbutton_clicked();
    void on_guesstheletterbutton_clicked();
    void on_guessthepasswordbutton_clicked();

    void createtableword();
    void setui();

    void debugger();
    void addwordstolist();

    void computerguesses();
    void endscreen();
    void setuiforpc(QString s1);
    void makewordlistshorter(QString s1);
    void conditionaloflistelements();



    void delay(int n);
    void betteralphabetinvec();
    void worsealphabetinvec();
    void randomchar();

    void on_giveupbutton_clicked();

    void on_easylevel_clicked();
    void on_normallevel_clicked();
    void on_hardlevel_clicked();

private:
    Ui::Guessmyword *ui;
    int gamecounter;
    int lettersleft;
    int level;

    int wordlength;
    QString generatedword;
    QString tableword;
    QVector <QString> generatedletters;
    bool ifexist;

    QVector <QString> listofwords;

    bool returnNonRandomChar();
    int index;


    QVector <QString> alphabet;
    QVector <QString> earLetters;
    QVector <QString> guessedletters;
    QString randchar;
    bool letterexist;
    int letterposition;

    int playerscore;
    int computerscore;
};

#endif // GUESSMYWORD_H
