#ifndef SHIPS_H
#define SHIPS_H

#include <QDialog>
#include <QVector>
#include <QPushButton>

namespace Ui {
class Ships;
}

class Ships : public QDialog
{
    Q_OBJECT

public:
    explicit Ships(QWidget *parent = nullptr);
    ~Ships();

private:          //buttons/difficulty level
    void on_actionButton_clicked();
    void disable_your_buttons();
    void disable_cpu_buttons();
    void enable_your_buttons();
    void enable_cpu_buttons();
    void disable_shot_buttons();
    void disable_playerships_buttons();
    void delay(int n);
    void on_easylevel_clicked();
    void on_normallevel_clicked();
    void on_hardlevel_clicked();

    void generatecomputerships();
    void createcomputerships(int n);

    void cpushot();
    void changecolorifhit();

    void winscreen();
    void cpuwinscreen();

    void on_computerbutton_clicked(QPushButton *p1);
     private slots:
    //Player ships
    void setshipsbuttonsclicked(QPushButton *p1);
    void on_A1_clicked();
    void on_D1_clicked();
    void on_B1_clicked();
    void on_E1_clicked();
    void on_C1_clicked();
    void on_F1_clicked();
    void on_A2_clicked();
    void on_D2_clicked();
    void on_B2_clicked();
    void on_E2_clicked();
    void on_C2_clicked();
    void on_F2_clicked();
    void on_A3_clicked();
    void on_D3_clicked();
    void on_B3_clicked();
    void on_E3_clicked();
    void on_C3_clicked();
    void on_F3_clicked();
    void on_A4_clicked();
    void on_D4_clicked();
    void on_B4_clicked();
    void on_E4_clicked();
    void on_C4_clicked();
    void on_F4_clicked();
    void on_A5_clicked();
    void on_D5_clicked();
    void on_B5_clicked();
    void on_E5_clicked();
    void on_C5_clicked();
    void on_F5_clicked();
    void on_A6_clicked();
    void on_D6_clicked();
    void on_B6_clicked();
    void on_E6_clicked();
    void on_C6_clicked();
    void on_F6_clicked();

    //CPU ships
    void on_C_A1_clicked();
    void on_C_B1_clicked();
    void on_C_C1_clicked();
    void on_C_D1_clicked();
    void on_C_E1_clicked();
    void on_C_F1_clicked();
    void on_C_A2_clicked();
    void on_C_B2_clicked();
    void on_C_C2_clicked();
    void on_C_D2_clicked();
    void on_C_E2_clicked();
    void on_C_F2_clicked();
    void on_C_A3_clicked();
    void on_C_B3_clicked();
    void on_C_C3_clicked();
    void on_C_D3_clicked();
    void on_C_E3_clicked();
    void on_C_F3_clicked();
    void on_C_A4_clicked();
    void on_C_B4_clicked();
    void on_C_C4_clicked();
    void on_C_D4_clicked();
    void on_C_E4_clicked();
    void on_C_F4_clicked();
    void on_C_A5_clicked();
    void on_C_B5_clicked();
    void on_C_C5_clicked();
    void on_C_D5_clicked();
    void on_C_E5_clicked();
    void on_C_F5_clicked();
    void on_C_A6_clicked();
    void on_C_B6_clicked();
    void on_C_C6_clicked();
    void on_C_D6_clicked();
    void on_C_E6_clicked();
    void on_C_F6_clicked();


private:
    Ui::Ships *ui;

    int number_of_action; //counting number of pressing actionButton
    int difficulty;

    int counter; //counting the number of set ships elements
    int needed; //required number of elements

    bool error;  //errors
    bool ifhit; //checking if hit

    int playerscore;
    int cpuscore;
    int cpuhelper;  //Didnt use just yet

    //Variables that are used to generate computer ships
    int cord1;  //firsthalfofcoordinates
    int cord2;  //secondhalfofcoordinates
    int numberOfShots;
    QString cord1s; //the same in Qstring
    QString cord2s; //the same in Qstring
    QString cord;   //coordinates connected/united;

    QVector <QString> computerships;
    QVector <QString> savedshipselements;
    QVector <QString> playerships;

    QVector <QString> computershots;
    QVector <QString> playershots;

};

#endif // SHIPS_H
