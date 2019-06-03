#ifndef TRAINYOURMEMORY_H
#define TRAINYOURMEMORY_H

#include <QDialog>

namespace Ui {
class Trainyourmemory;
}

class Trainyourmemory : public QDialog
{
    Q_OBJECT

public:
    explicit Trainyourmemory(QWidget *parent = nullptr);
    ~Trainyourmemory();

private:
    Ui::Trainyourmemory *ui;
};

#endif // TRAINYOURMEMORY_H
