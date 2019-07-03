#include <iostream>
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

	double x; char aux; double y;

	cout << "Введите выражение." << endl;
	while (cout << "> " && !safe(cin >> x >> aux >> y) || !(aux == '+' || aux == '-' || aux == '*' || aux == '/'));
	cout << endl;

	cout << "Результат: ";
	switch (aux)
	{
	case '+': cout << x + y << endl; break;
	case '-': cout << x - y << endl; break;
	case '*': cout << x * y << endl; break;
	case '/': cout << x / y << endl; break;
	}
	cout << endl;

	return 0;
}