#ifndef SHIPS_H
#define SHIPS_H

#include <QDialog>

namespace Ui {
class Ships;
}

class Ships : public QDialog
{
    Q_OBJECT

public:
    explicit Ships(QWidget *parent = nullptr);
    ~Ships();

private:
    Ui::Ships *ui;
};

#endif // SHIPS_H
