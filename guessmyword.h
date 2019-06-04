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

private:
    Ui::Guessmyword *ui;
};

#endif // GUESSMYWORD_H
