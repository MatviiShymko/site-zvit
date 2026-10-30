/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task2.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 1
// Запланована ціль: лабораторна робота №4, завдання 2, варіант 6.
//                   Обчислити нескінченний ланцюговий дріб для
//                   кореня з 3 із заданою точністю.
/////////////////////////////////////////////////////////////////////

#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

//=========== константи
const float MIN_ACCURACY = 0.00001f; // найменша точність
const float MAX_ACCURACY = 0.1f;     // найбільша точність

//=========== глобальні змінні
float accuracy = 0;      // точність
float current = 0;       // значення дробу для поточної кількості рівнів
float previous = 0;      // значення дробу на попередньому кроці
float difference = 0;    // модуль різниці двох сусідніх значень
int levelCount = 0;      // кількість рівнів дробу
float standardValue = 0; // значення кореня через sqrt

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 4. Task 2. Variant 6." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;

    cout << "Calculate the infinite continued fraction for the root of 3"
         << endl;
    cout << "sqrt(3) = 1 + 1/(1 + 1/(2 + 1/(1 + 1/(2 + ...))))" << endl << endl;
}

/*
 * Функція вводить точність. Якщо введено не число або число
 * поза межами, введення повторюється
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputAccuracy()
{
    cout << "Enter accuracy (from 0.00001 to 0.1): ";
    cin >> accuracy;
    while (cin.fail() || accuracy < MIN_ACCURACY || accuracy > MAX_ACCURACY)
    {
        cin.clear();            // скинути помилку введення
        cin.ignore(1000, '\n'); // і пропустити неправильний рядок
        cout << "Error! Enter accuracy from 0.00001 to 0.1: ";
        cin >> accuracy;
    }
}

/*
 * Функція рахує дріб із заданою кількістю рівнів, починаючи знизу.
 * Знаменники під першою одиницею чергуються: 1, 2, 1, 2, ...
 * Параметри: levels - кількість рівнів дробу (не менше 1)
 * Значення, що повертаються: значення дробу
 */
float calculateFraction(int levels)
{
    float value = 2; // значення знаменника на найнижчому рівні

    if (levels % 2 == 1)
    {
        value = 1;
    }

    for (int i = levels - 1; i >= 1; i--)
    {
        if (i % 2 == 1)
        {
            value = 1 + 1 / value;
        }
        else
        {
            value = 2 + 1 / value;
        }
    }

    return 1 + 1 / value;
}

/*
 * Функція збільшує кількість рівнів дробу, поки два сусідні значення
 * не стануть ближчими за точність, і виводить таблицю
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void calculateRoot()
{
    cout << endl << fixed << setprecision(6);
    cout << setw(8) << "levels" << setw(14) << "value" << setw(14)
         << "difference" << endl;

    levelCount = 1;
    current = calculateFraction(levelCount);
    cout << setw(8) << levelCount << setw(14) << current << endl;

    do
    {
        previous = current;
        levelCount++;
        current = calculateFraction(levelCount);
        difference = fabs(current - previous);
        cout << setw(8) << levelCount << setw(14) << current << setw(14)
             << difference << endl;
    } while (difference > accuracy);
}

/*
 * Функція порівнює результат із значенням sqrt(3)
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void printResult()
{
    standardValue = sqrt(3.0f);

    cout << endl << "Continued fraction = " << current << endl;
    cout << "sqrt(3)            = " << standardValue << endl;
    cout << scientific << setprecision(2);
    cout << "Error              = " << fabs(current - standardValue) << endl;
    cout << "Number of levels   = " << levelCount << endl;
}

int main()
{
    welcome();
    inputAccuracy();
    calculateRoot();
    printResult();
    return 0;
}
