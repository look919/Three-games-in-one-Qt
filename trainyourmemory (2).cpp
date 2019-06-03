#include "trainyourmemory.h"
#include "ui_trainyourmemory.h"

Trainyourmemory::Trainyourmemory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Trainyourmemory)
{
    ui->setupUi(this);
}

Trainyourmemory::~Trainyourmemory()
{
    delete ui;
}
