#include "rules.h"
#include "ui_rules.h"

Rules::Rules(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Rules)
{
    ui->setupUi(this);
}

Rules::~Rules()
{
    delete ui;
}

void Rules::on_quitButton_clicked()
{
    this->close();
}

void Rules::on_guessMyWordButton_clicked()
{
    ui->textEdit->clear();
    ui->textEdit->setText("Game takes part in two rounds. In first you are trying to guess a word that computer generated for you, "
      "firstly you can ask him for 10 letters, if the word contain them you will show it on your screen with their position in generated word. "
      "If u are sure of your answer, fill the field on your right, but remember you have only one shot! "
      "In second round you create your own word, but there are some rules, "
      "it must contain from 5 to 12 letters and you cannot use proper names, after that its computer time to guess. "
      "Wins a person or machine who answers correctly and uses fewer letters. Goodluck!");
    ui->textEdit->setAlignment(Qt::AlignCenter);
}

void Rules::on_shipsButton_clicked()
{
    ui->textEdit->clear();
    ui->textEdit->setText("First of all u place your ships on the board, you can place them near anothers, "
                          "but its forbidden to put the fragemnts of one ships seperetaly, even thought it doesn't change anyting to cpu, "
                          "after that u begin to play, you always start first, keep that in mind. "
                          "Wins a person or machine who firstly shot all three ships of enemy fleet. Goodluck!");

    ui->textEdit->setAlignment(Qt::AlignCenter);
}

void Rules::on_trainYourMemoryButton_clicked()
{
    ui->textEdit->clear();
    ui->textEdit->setText("It's simply game that bases on memoring. On the beggining you observe changes of color on buttons, "
                          "then you have to repeat thatcombination in the same order, scheme of that changes in each round. "
                          "In the first round you have to remember 3 changes to green, then one more with each next round. "
                          "After fourth round button can change it's color to red, that doesn't count to the combination. "
                          "How far can you go? Goodluck!");
    ui->textEdit->setAlignment(Qt::AlignCenter);
}
