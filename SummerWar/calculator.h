#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>

namespace Ui {
class Calculator;
}

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:

    void on_num0_clicked();
    void on_num1_clicked();
    void on_num2_clicked();
    void on_num3_clicked();
    void on_num4_clicked();
    void on_num5_clicked();
    void on_num6_clicked();
    void on_num7_clicked();
    void on_num8_clicked();
    void on_num9_clicked();
    void on_Comma_clicked();

    void on_Pi_clicked();
    void on_Exponent_clicked();

    void on_Plus_clicked();
    void on_Minus_clicked();
    void on_Division_clicked();
    void on_Multiplication_clicked();

    void on_Root_clicked();
    void on_Power_clicked();
    void on_Logarithm_clicked();
    void on_SquareRoot_clicked();

    void on_ParenLeft_clicked();
    void on_ParenRight_clicked();

    void on_Calculate_clicked();

    void on_Clear_clicked();
    void on_Delete_clicked();

    void on_Redo_clicked();
    void on_Undo_clicked();

private:
    Ui::Calculator *ui;

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // CALCULATOR_H
