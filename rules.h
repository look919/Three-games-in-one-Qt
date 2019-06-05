#ifndef RULES_H
#define RULES_H

#include <QDialog>

namespace Ui {
class Rules;
}

class Rules : public QDialog
{
    Q_OBJECT

public:
    explicit Rules(QWidget *parent = nullptr);
    ~Rules();

private slots:
    void on_quitButton_clicked();

    void on_guessMyWordButton_clicked();

    void on_shipsButton_clicked();

    void on_trainYourMemoryButton_clicked();

private:
    Ui::Rules *ui;
};

#endif // RULES_H
