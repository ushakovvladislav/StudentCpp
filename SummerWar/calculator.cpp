#include "calculator.h"
#include "ui_calculator.h"
#include <QKeyEvent>
#include <cmath>

Calculator::Calculator(QWidget *parent) : QMainWindow(parent), ui(new Ui::Calculator)
{
    ui->setupUi(this);
}
Calculator::~Calculator()
{
    delete ui;
}

using::std::string;
using::std::vector;

template<class T>
bool is_in(string where, T what)
{
    for (auto& temp : where)
        if (what == temp) return true;
    return false;
}
template <class T>
bool is_num(T it)
{
    return (it == '.' || it >= '0' && it <= '9');
}

static string expr;
int paren_difference()
{
    int pd = 0;
    for (auto p = expr.begin(); p < expr.end(); p++)
        (*p == '(') ? pd++ :
        (*p == ')') ? pd-- : 0;
    return pd;
}
bool last_is_float()
{
    for (auto p = expr.rbegin(); p < expr.rend() && is_num(*p); p++)
        if (*p == '.') return true;
    return false;
}
bool last_is_func()
{
    for (auto p = expr.rbegin(); p < expr.rend(); p++)
    {
        if (is_in("+-*/(", *p)) return false;
        if (is_in("^#&", *p)) return true;
    }
    return false;
}

static vector<string> step = {string()};
static unsigned int current = 0;
void new_step()
{
    step.resize(++current + 1);
    step[current] = expr;
}
void undo()
{
    if (current) expr = step[--current];
}
void redo()
{
    if (current + 1 < step.size()) expr = step[++current];
}

void add(char it)
{
    if (expr.length())
    {
        if (expr.length() >= 64) return;

        if (is_num(it))
        {
            if (it == '.' && last_is_float()) return;
            if (is_in("pe)", expr.back())) expr += '*';
        }
        else if (is_in("pe(", it))
        {
            if ((expr.length() < 2 || expr.length() > 2
            && is_in("*/^#&(", expr[expr.length() - 2]))
            && is_in("+-", expr[expr.length() - 1])) expr += "1*";
            if (is_num(expr.back()) || is_in("pe)", expr.back())) expr += '*';
        }
        else if (is_in("+-", it))
        {
            if (is_in("+-", expr.back())) return;
        }
        else if (is_in("*/)", it))
        {
            if (it == ')' && (!paren_difference() || expr.back() == '(')) return;
            if (!is_num(expr.back()) && !is_in("pe)", expr.back())) return;
        }
        else if (is_in("^#&", it))
        {
            if (last_is_func() || !is_num(expr.back()) && !is_in("pe)", expr.back())) return;
        }
    }
    else if (!is_num(it) && !is_in("pe+-(", it)) return;

    expr += it;
    new_step();
}
QString formatted()
{
    QString text = "<p align=\"right\">";

    for (unsigned int i = 0; i < expr.length(); i++)
    {
        if ((i == 0 || i > 0 && !is_num(expr[i - 1])) && expr[i] == '2' && expr[i + 1] == '#') continue;
        if (expr[i] == 'p') text += "π";
        else if (expr[i] == '^' || expr[i] == '#' || expr[i] == '&')
        {
            if (expr[i] == '#' || expr[i] == '&')
            {
                int here = text.length() - 1;
                if (text[here] == ')')
                {
                    int tmp = 0;
                    do {
                        if (text[here] == '(') tmp++;
                        if (text[here] == ')') tmp--;
                    }
                    while (--here >= 0 && tmp);
                }
                else
                {
                    do {
                        if (!(text[here] == '.' || text[here] >= '0' && text[here] <= '9')
                        && text[here] != 'e' && text[here] != "π") break;
                    }
                    while (--here >= 0);
                }

                if (expr[i] == '#')
                {
                    text.insert(here + 1, "<sup>");
                    text += "</sup>";
                }
                else
                {
                    text.insert(here + 1, "log<sub>");
                    text += "</sub>";
                }
            }
            if (expr[i] == '^' || expr[i] == '#')
            {
                unsigned int here = i;
                if (expr[i] == '^')
                {
                    if (i + 1 < expr.length()) text += "<sup>";
                    else {text += "^"; break;}
                }
                else
                {
                    if (i + 1 < expr.length()) text += "√<span style=\"text-decoration: overline\">";
                    else {text += "√"; break;}
                }
                if (expr[++i] == '+' || expr[i] == '-') text += expr[i];
                else i--;
                if (expr[++i] == '(')
                {
                    int tmp = 0;
                    do {
                        if (expr[i] == '(') tmp++;
                        if (expr[i] == ')') tmp--;
                        if (expr[i] == 'p') text += "π";
                        else text += expr[i];
                    }
                    while (++i < expr.length() && tmp);
                }
                else
                {
                    do {
                        if (!(expr[i] == '.' || expr[i] >= '0' && expr[i] <= '9')
                        && expr[i] != 'e' && expr[i] != 'p') break;
                        if (expr[i] == 'p') text += "π";
                        else text += expr[i];
                    }
                    while (++i < expr.length());
                }
                i--;
                if (expr[here] == '^') text += "</sup>";
                else text += "</span>";
            }
        }
        else text += expr[i];
    }

    text += "<span style=\"color: #CCCCCC\">";
    for (int i = 0; i < paren_difference(); i++) text+= ')';
    text += "</span>";

    return text;
}

int precedence(char it)
{
    switch (it)
    {
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    case '^': case '#': case '&': return 3;
    }
    return 0;
}
void calculate()
{
    while (paren_difference()) expr += ")";
    if ((expr.length() > 1 && expr[expr.length() - 2] == '(' && expr[expr.length() - 1] == ')')
    || !expr.length() || is_in("+-*/^#&", expr.back()))
    {
        expr = string();
        new_step();
        return;
    }

    vector<char> rev;
    vector<double> num;
    vector<char> tmp; tmp.push_back('0');

    for (unsigned int i = 0; i < expr.length(); i++)
    {

        if (is_num(expr[i]) || (i == 0 || is_in("*/^#&(", expr[i - 1])) && is_in("+-", expr[i]))
        {
            rev.push_back(num.size());
            num.push_back(atof(expr.data() + i++));
            while (is_num(expr[i])) i++;

        }
        if (expr[i] == 'p')
        {
            rev.push_back(num.size());
            num.push_back(3.141593);
            i++;
        }
        if (expr[i] == 'e')
        {
            rev.push_back(num.size());
            num.push_back(2.718282);
            i++;
        }
        if (is_in("+-*/^#&", expr[i]))
        {
            if (precedence(expr[i]) <= precedence(tmp.back()))
            {
                rev.push_back(tmp.back());
                tmp.pop_back();
                tmp.push_back(expr[i]);
            }
            else tmp.push_back(expr[i]);
        }
        if (expr[i] == '(')
        {
            tmp.push_back(expr[i]);
        }
        if (expr[i] == ')')
        {
            while (tmp.back() != '(')
            {
                rev.push_back(tmp.back());
                tmp.pop_back();
            }
            tmp.pop_back();
        }
    }
    while (tmp.back() != '0')
    {
        rev.push_back(tmp.back());
        tmp.pop_back();
    }
    int len = rev.size();
    for (unsigned char i = 0; i < len && len > 1; len -= 2, i = 0)
    {
        while (rev[i] < 32) i++;
        switch (rev[i])
        {
        case '+': num[rev[i - 2]] += num[rev[i - 1]]; break;
        case '-': num[rev[i - 2]] -= num[rev[i - 1]]; break;
        case '*': num[rev[i - 2]] *= num[rev[i - 1]]; break;
        case '/': num[rev[i - 2]] /= num[rev[i - 1]]; break;
        case '^': num[rev[i - 2]] = pow(num[rev[i - 2]], num[rev[i - 1]]); break;
        case '#': num[rev[i - 2]] = pow(num[rev[i - 1]], 1 / num[rev[i - 2]]); break;
        case '&': num[rev[i - 2]] = log(num[rev[i - 1]]) / log(num[rev[i - 2]]); break;
        }
        for (unsigned char j = 0, k = 0; j < len; j++)
            if (j != i - 1 && j != i) rev[k++] = rev[j];
    }
    expr = std::to_string(num[0]);
    if (last_is_float())
        for (auto p = expr.rbegin(); p < expr.rend(); p++)
            if (*p == '0') expr.pop_back();
            else if (*p == '.') {expr.pop_back(); break;}
            else break;
    new_step();
}

void Calculator::on_num0_clicked()           {add('0'); ui->Output->setText(formatted());}
void Calculator::on_num1_clicked()           {add('1'); ui->Output->setText(formatted());}
void Calculator::on_num2_clicked()           {add('2'); ui->Output->setText(formatted());}
void Calculator::on_num3_clicked()           {add('3'); ui->Output->setText(formatted());}
void Calculator::on_num4_clicked()           {add('4'); ui->Output->setText(formatted());}
void Calculator::on_num5_clicked()           {add('5'); ui->Output->setText(formatted());}
void Calculator::on_num6_clicked()           {add('6'); ui->Output->setText(formatted());}
void Calculator::on_num7_clicked()           {add('7'); ui->Output->setText(formatted());}
void Calculator::on_num8_clicked()           {add('8'); ui->Output->setText(formatted());}
void Calculator::on_num9_clicked()           {add('9'); ui->Output->setText(formatted());}
void Calculator::on_Comma_clicked()          {add('.'); ui->Output->setText(formatted());}

void Calculator::on_Pi_clicked()             {add('p'); ui->Output->setText(formatted());}
void Calculator::on_Exponent_clicked()       {add('e'); ui->Output->setText(formatted());}

void Calculator::on_Plus_clicked()           {add('+'); ui->Output->setText(formatted());}
void Calculator::on_Minus_clicked()          {add('-'); ui->Output->setText(formatted());}
void Calculator::on_Division_clicked()       {add('/'); ui->Output->setText(formatted());}
void Calculator::on_Multiplication_clicked() {add('*'); ui->Output->setText(formatted());}

void Calculator::on_Root_clicked()           {add('#'); ui->Output->setText(formatted());}
void Calculator::on_Power_clicked()          {add('^'); ui->Output->setText(formatted());}
void Calculator::on_Logarithm_clicked()      {add('&'); ui->Output->setText(formatted());}
void Calculator::on_SquareRoot_clicked()     {add('*'); add('2'); add('#'); ui->Output->setText(formatted());}

void Calculator::on_ParenLeft_clicked()      {add('('); ui->Output->setText(formatted());}
void Calculator::on_ParenRight_clicked()     {add(')'); ui->Output->setText(formatted());}

void Calculator::on_Calculate_clicked()      {calculate(); ui->Output->setText(formatted());}

void Calculator::on_Clear_clicked()          {if (expr.length()) {expr = string(); new_step();} ui->Output->setText(formatted());}
void Calculator::on_Delete_clicked()         {if (expr.length()) {expr.pop_back(); new_step();} ui->Output->setText(formatted());}

void Calculator::on_Redo_clicked()           {redo(); ui->Output->setText(formatted());}
void Calculator::on_Undo_clicked()           {undo(); ui->Output->setText(formatted());}

void Calculator::keyPressEvent(QKeyEvent *pressed)
{
    switch (pressed->key())
    {
    case Qt::Key_0: on_num0_clicked(); break;
    case Qt::Key_1: on_num1_clicked(); break;
    case Qt::Key_2: on_num2_clicked(); break;
    case Qt::Key_3: on_num3_clicked(); break;
    case Qt::Key_4: on_num4_clicked(); break;
    case Qt::Key_5: on_num5_clicked(); break;
    case Qt::Key_6: on_num6_clicked(); break;
    case Qt::Key_7: on_num7_clicked(); break;
    case Qt::Key_8: on_num8_clicked(); break;
    case Qt::Key_9: on_num9_clicked(); break;
    case Qt::Key_Comma:
    case Qt::Key_Period: on_Comma_clicked(); break;

    case Qt::Key_P: on_Pi_clicked(); break;
    case Qt::Key_E: on_Exponent_clicked(); break;

    case Qt::Key_Plus: on_Plus_clicked(); break;
    case Qt::Key_Minus: on_Minus_clicked(); break;
    case Qt::Key_Slash: on_Division_clicked(); break;
    case Qt::Key_Asterisk: on_Multiplication_clicked(); break;

    case Qt::Key_AsciiCircum: on_Power_clicked(); break;

    case Qt::Key_ParenLeft: on_ParenLeft_clicked(); break;
    case Qt::Key_ParenRight: on_ParenRight_clicked(); break;

    case Qt::Key_Enter:
    case Qt::Key_Return: on_Calculate_clicked(); break;

    case Qt::Key_Delete: on_Clear_clicked(); break;
    case Qt::Key_Backspace: on_Delete_clicked(); break;

    case Qt::Key_Y: if (pressed->modifiers() == Qt::ControlModifier) on_Redo_clicked(); break;
    case Qt::Key_Z: if (pressed->modifiers() == Qt::ControlModifier) on_Undo_clicked(); break;
    }
}
