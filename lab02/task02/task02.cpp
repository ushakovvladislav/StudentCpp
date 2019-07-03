#include <iostream>
using namespace std;

int main()
{
	setlocale(2, "Russian");

	cout << "Добро пожаловать!" << endl;

	char* aux = new char[32];
	double* num = new double[32]; unsigned int n = 0;

	cout << "Введите числа." << endl;
	while (true)
	{
		cout << "> "; cin.getline(aux, 32);
		if (aux[0] == 'e' && aux[1] == 'n' && aux[2] == 'd') break;
		num[n++] = atof(aux);
	}
	cout << endl;

	num = static_cast<double*>(realloc(num, n * sizeof(double)));

	cout << "Введите операции." << endl;
	cout << "> "; cin.getline(aux, 32);
	cout << endl;

	aux = static_cast<char*>(realloc(aux, (strlen(aux) + 1) * sizeof(char)));

	cout << "Результат: ";
	for (unsigned int i = 0; i < n - 1 && i < strlen(aux); i++)
	{
		switch (aux[i])
		{
		case '+': num[0] += num[i + 1]; break;
		case '-': num[0] -= num[i + 1]; break;
		case '*': num[0] *= num[i + 1]; break;
		case '/': num[0] /= num[i + 1]; break;
		}
	}
	cout << num[0];
	cout << endl;

	delete[] aux;
	delete[] num;

	return 0;
}