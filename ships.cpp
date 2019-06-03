#include "ships.h"
#include "ui_ships.h"

Ships::Ships(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ships)
{
    ui->setupUi(this);
}

Ships::~Ships()
{
    delete ui;
}
