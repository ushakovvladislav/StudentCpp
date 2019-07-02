#include <iostream>
#include <cmath>
using namespace std;

bool safe(istream& in)
{
	if (in.peek() != '\n')
	{
		while (in.get() != '\n') in.clear();
		return false;
	}
	return true;
}

int main()
{
	setlocale(2, "Russian");

	cout << "Добро пожаловать!" << endl;

	char aux = 'y';
	while (aux == 'y')
	{
		cout << "Введите числа." << endl;
		double x; while (cout << "x = " && !safe(cin >> x));
		double y; while (cout << "y = " && !safe(cin >> y));
		cout << endl;

		cout << "Выберите операцию." << endl;
		cout << "1. x + y" << endl;
		cout << "2. x - y" << endl;
		cout << "3. x * y" << endl;
		cout << "4. x / y" << endl;
		cout << "5. x ^ y" << endl;
		cout << "6. y # x" << endl;
		cout << "7. x & y" << endl;
		while (cout << "> " && !safe(cin >> aux) || !(aux >= '1' && aux <= '7'));
		cout << endl;

		cout << "Результат: ";
		switch (aux)
		{
		case '1': cout << x + y << endl; break;
		case '2': cout << x - y << endl; break;
		case '3': cout << x * y << endl; break;
		case '4': cout << x / y << endl; break;
		case '5': cout << pow(x, y) << endl; break;
		case '6': cout << pow(x, 1 / y) << endl; break;
		case '7': cout << log(x) / log(y) << endl; break;
		}
		cout << endl;

		cout << "Продолжить?" << endl;
		cout << "y - да" << endl;
		cout << "n - нет" << endl;
		while (cout << "> " && !safe(cin >> aux) || !(aux == 'y' || aux == 'n'));
		cout << endl;
	}
	return 0;
}