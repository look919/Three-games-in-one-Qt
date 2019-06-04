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
    ui->textEdit->setText("Game takes part in two rounds. In first you are trying to guess a word that computer generated for you, "
      "firstly you can ask him for 10 letters, if the word contain them you will show it on your screen with their position in generated word. "
      "If u are sure of your answer, fill the field on your right, but remember you have only one shot! "
      "In second round you create your own word, but there are some rules, "
      "it must contain from 5 to 12 letters and you cannot use proper names, after that its computer time to guess. "
      "Wins a person or machine who answered correctly and used fewer letters. Goodluck!");
    ui->textEdit->setAlignment(Qt::AlignCenter);
}

void Rules::on_shipsButton_clicked()
{

}

void Rules::on_trainYourMemoryButton_clicked()
{

}
