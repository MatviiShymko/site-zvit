/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task1.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 2
// Запланована ціль: лабораторна робота №2, завдання 1, варіант 6.
//                   Обчислити значення функції f, формула якої
//                   залежить від добутку ab.
/////////////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

//=========== константи
const float BORDER = 10;     // межа добутку ab з умови
const float MULTIPLIER = 10; // множник у формулі 10a^2 / b

//=========== глобальні змінні
float a = 0;       // перший аргумент
float b = 0;       // другий аргумент
float product = 0; // добуток ab
float value = 0;   // значення функції f

/*
 * Функція вводить a і b, вибирає формулу за значенням добутку ab
 * і виводить значення f. Для ab < 0 і ab = 10 функція не визначена
 * Параметри: немає
 * Значення, що повертаються: 0
 */
int main()
{
    cout << "Lab work 2. Task 1. Variant 6." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;
    cout << "Calculate the function" << endl;
    cout << "     b^2 / (10 - ab),  0 < ab < 10" << endl;
    cout << "f =  10a^2 / b,        ab > 10" << endl;
    cout << "     1,                ab = 0" << endl << endl;

    cout << "Enter a and b: ";
    cin >> a >> b;
    while (cin.fail())
    {
        cin.clear();            // скинути помилку введення
        cin.ignore(1000, '\n'); // і пропустити неправильний рядок
        cout << "Error! Enter two numbers: ";
        cin >> a >> b;
    }

    product = a * b;
    cout << "ab = " << product << endl;

    if (product < 0 || product == BORDER)
    {
        cout << "The function is not defined for these a and b." << endl;
    }
    else
    {
        if (product == 0)
        {
            value = 1;
        }
        else if (product < BORDER)
        {
            value = b * b / (BORDER - product);
        }
        else
        {
            value = MULTIPLIER * a * a / b;
        }

        cout << "f = " << value << endl;
    }

    return 0;
}
