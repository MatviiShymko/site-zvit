/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task1.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 2
// Запланована ціль: лабораторна робота №3, варіант 8.
//                   Обчислити суму ряду із заданою точністю
//                   і вивести таблицю обчислень.
/////////////////////////////////////////////////////////////////////

#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

//=========== константи
const float MIN_ACCURACY = 0.000001f; // найменша допустима точність
const float MAX_ACCURACY = 1;         // найбільша допустима точність
const float LOWER_LIMIT = 1e-38f;     // межі модуля члена ряду з умови
const float UPPER_LIMIT = 1e38f;

//=========== глобальні змінні
float accuracy = 0;  // точність
float sum = 0;       // накопичена сума
float factorial = 1; // значення x!
float member = 0;    // поточний член ряду
int x = 0;           // параметр зовнішньої суми
int k = 0;           // параметр внутрішньої суми
int termCount = 0;   // кількість доданків для поточного x

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 3. Variant 8." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;

    cout << "Calculate the sum of the series" << endl;
    cout << "      5    inf     (-1)^k * (x+1)^(k-1)" << endl;
    cout << "S =  sum   sum   --------------------------" << endl;
    cout << "     x=1   k=0   (k+1) * (k+2) * ... * (k+x)" << endl << endl;
}

/*
 * Функція вводить точність. Якщо введено не число або число
 * поза межами, введення повторюється
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputAccuracy()
{
    cout << "Enter accuracy (from 0.000001 to 1): ";
    cin >> accuracy;
    while (cin.fail() || accuracy < MIN_ACCURACY || accuracy > MAX_ACCURACY)
    {
        cin.clear();            // скинути помилку введення
        cin.ignore(1000, '\n'); // і пропустити неправильний рядок
        cout << "Error! Enter accuracy from 0.000001 to 1: ";
        cin >> accuracy;
    }
}

/*
 * Функція рахує суму ряду і виводить таблицю. Для кожного x від 1 до 5
 * внутрішня сума йде, поки член ряду більший за точність і не вийшов
 * за межі 10^-38 та 10^38. Кожен член рахується через попередній
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void calculateSum()
{
    cout << endl << scientific << setprecision(6);
    cout << setw(3) << "x" << setw(6) << "k" << setw(16) << "member" << setw(16)
         << "sum" << endl;

    sum = 0;
    factorial = 1;
    for (x = 1; x <= 5; x++)
    {
        factorial = factorial * x; // x! = (x-1)! * x
        k = 0;
        termCount = 0;
        member = 1 / ((x + 1) * factorial); // перший член, коли k = 0

        do
        {
            sum = sum + member;
            termCount++;
            cout << setw(3) << x << setw(6) << k << setw(16) << member
                 << setw(16) << sum << endl;

            k++;
            // спочатку ділення, щоб проміжне значення не вийшло за межі float
            member = -member * (x + 1) / (k + x) * k;
        } while (fabs(member) > accuracy && fabs(member) > LOWER_LIMIT
                 && fabs(member) < UPPER_LIMIT);

        cout << "number of terms for x = " << x << ": " << termCount << endl;
    }

    cout << endl << "S = " << sum << endl;
    cout << "The series diverges, so S is a partial sum." << endl;
}

int main()
{
    welcome();
    inputAccuracy();
    calculateSum();
    return 0;
}
