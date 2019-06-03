#ifndef MEMORY_H
#define MEMORY_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class Trainyourmemory;
}

class Trainyourmemory : public QDialog
{
    Q_OBJECT

public:
    explicit Trainyourmemory(QWidget *parent = nullptr);
    ~Trainyourmemory();

private slots:

    void startRound();

    void on_anybutton_clicked(QPushButton *n);
    void on_button1_clicked();
    void on_button2_clicked();
    void on_button3_clicked();
    void on_button4_clicked();
    void on_button5_clicked();
    void on_button6_clicked();
    void on_button7_clicked();
    void on_button8_clicked();
    void on_button9_clicked();

    void paintRandomButtonOnGreen();
    void paintRandomButtonOnRed();
    void cleanButtons();
    void wonround();
    void mistake();

    void enable_buttons();
    void disable_buttons();
    void delay(int n);

private:
    Ui::Trainyourmemory *ui;
    bool playGame;
    int roundCounter;

    QString clickedButtonName;
    QVector<QString> buttons;
    int index;           //this i use to check if player clicks on the right buttons
    int lastelement;      //last element in each round
};

#endif // MEMORY_H
