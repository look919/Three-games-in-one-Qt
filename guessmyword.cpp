#include "guessmyword.h"
#include "ui_guessmyword.h"

Guessmyword::Guessmyword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Guessmyword)
{
    ui->setupUi(this);
}

Guessmyword::~Guessmyword()
{
    delete ui;
}
