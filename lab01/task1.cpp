/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task1.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 2
// Запланована ціль: лабораторна робота №1, завдання 1, варіант 2.
//                   Обчислити значення виразу
//                   X = (2.3 + AB^2) / (0.4 + Ce^D).
/////////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>
using namespace std;

//=========== константи
const float NUMERATOR_TERM = 2.3f;   // доданок у чисельнику
const float DENOMINATOR_TERM = 0.4f; // доданок у знаменнику
const float ZERO_LIMIT = 0.000001f;  // менший за це модуль вважається нулем
const float MAX_VALUE = 1e38f;       // межа значень типу float

//=========== глобальні змінні
float a = 0;           // значення A
float b = 0;           // значення B
float c = 0;           // значення C
float d = 0;           // значення D
float numerator = 0;   // чисельник виразу
float denominator = 0; // знаменник виразу
float result = 0;      // значення X

/*
 * Функція вводить A, B, C, D, перевіряє знаменник на нуль
 * і виводить значення виразу X
 * Параметри: немає
 * Значення, що повертаються: 0
 */
int main()
{
    cout << "Lab work 1. Task 1. Variant 2." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;
    cout << "Calculate X = (2.3 + A * B^2) / (0.4 + C * e^D)" << endl << endl;

    cout << "Enter A, B, C, D: ";
    cin >> a >> b >> c >> d;
    while (cin.fail())
    {
        cin.clear();            // скинути помилку введення
        cin.ignore(1000, '\n'); // і пропустити неправильний рядок
        cout << "Error! Enter four numbers: ";
        cin >> a >> b >> c >> d;
    }

    numerator = NUMERATOR_TERM + a * b * b;
    denominator = DENOMINATOR_TERM + c * exp(d);

    if (fabs(denominator) < ZERO_LIMIT)
    {
        cout << "Error! The denominator is 0, X is not defined." << endl;
    }
    else
    {
        result = numerator / denominator;

        // якщо значення не вмістилось у float, ця умова хибна
        if (fabs(result) < MAX_VALUE)
        {
            cout << "X = " << result << endl;
        }
        else
        {
            cout << "Error! The numbers are too large for float." << endl;
        }
    }

    return 0;
}
