#include <iostream>
#include <cmath>
using namespace std;

int main()
{
	setlocale(2, "Russian");

	cout << "Добро пожаловать!" << endl;

	char aux = 'y';
	while (aux == 'y')
	{
		cout << "Введите числа." << endl;
		cout << "x = "; double x; cin >> x;
		cout << "y = "; double y; cin >> y;
		cout << endl;

		cout << "Выберите операцию." << endl;
		cout << "1. x + y" << endl;
		cout << "2. x - y" << endl;
		cout << "3. x * y" << endl;
		cout << "4. x / y" << endl;
		cout << "5. x ^ y" << endl;
		cout << "6. y # x" << endl;
		cout << "7. x & y" << endl;
		cout << "> "; cin >> aux;
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
		cout << "> "; cin >> aux;
		cout << endl;
	}
	return 0;
}