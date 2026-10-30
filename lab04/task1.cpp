/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task1.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 1
// Запланована ціль: лабораторна робота №4, завдання 1, варіант 6.
//                   Обчислити кусково задану функцію, розвинувши
//                   cos(x) у ряд Тейлора, і вивести таблицю.
/////////////////////////////////////////////////////////////////////

#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

//=========== константи
const float MIN_X = -10;                // найменше значення аргументу
const float MAX_X = 10;                 // найбільше значення аргументу
const float MIN_STEP = 0.01f;           // найменший крок
const float MAX_STEP = 10;              // найбільший крок
const float MIN_ACCURACY = 0.000001f;   // найменша точність
const float MAX_ACCURACY = 0.01f;       // найбільша точність
const float MIN_DENOMINATOR = 0.0001f;  // менший модуль вважається нулем
const float STEP_ROUNDING = 0.001f;     // поправка до кількості кроків
const float BORDER_ROUNDING = 0.00001f; // поправка до меж проміжків

//=========== глобальні змінні
float minX = 0;          // початкове значення x
float maxX = 0;          // кінцеве значення x
float step = 0;          // крок зміни x
float accuracy = 0;      // точність ряду
float x = 0;             // поточне значення аргументу
float y = 0;             // значення функції через ряд
float standardValue = 0; // значення через бібліотечні функції
float deviation = 0;     // похибка
int stepCount = 0;       // кількість кроків
bool isDefined = true;   // чи визначена функція в точці x

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 4. Task 1. Variant 6." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;

    cout << "Calculate the function, using the Taylor series of cos(x)" << endl;
    cout << "    | cos(x)^2 * cos(x^2),      -1 <= x <= 1" << endl;
    cout << "y = |" << endl;
    cout << "    | cos(x/2) / cos(x + 0.5),   1 <= x <= 2" << endl << endl;
}

/*
 * Функція вводить дійсне число. Якщо введено не число або число
 * поза межами, введення повторюється
 * Параметри: minValue - найменше допустиме значення,
 *            maxValue - найбільше допустиме значення
 * Значення, що повертаються: введене число
 */
float inputNumber(float minValue, float maxValue)
{
    float number = 0;

    cin >> number;
    while (cin.fail() || number < minValue || number > maxValue)
    {
        cin.clear();            // скинути помилку введення
        cin.ignore(1000, '\n'); // і пропустити неправильний рядок
        cout << "Error! Enter a number from the given range: ";
        cin >> number;
    }

    return number;
}

/*
 * Функція вводить межі зміни x, крок і точність
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputData()
{
    cout << "Enter minimal x (from -10 to 10): ";
    minX = inputNumber(MIN_X, MAX_X);

    cout << "Enter maximal x (not less than minimal, up to 10): ";
    maxX = inputNumber(minX, MAX_X);

    cout << "Enter step (from 0.01 to 10): ";
    step = inputNumber(MIN_STEP, MAX_STEP);

    cout << "Enter accuracy (from 0.000001 to 0.01): ";
    accuracy = inputNumber(MIN_ACCURACY, MAX_ACCURACY);
}

/*
 * Функція рахує cos через ряд Тейлора. Кожен член рахується через
 * попередній, поки модуль члена більший за точність
 * Параметри: argument - аргумент косинуса
 * Значення, що повертаються: сума ряду
 */
float calculateCos(float argument)
{
    float member = 1; // перший член ряду, коли n = 0
    float sum = 1;    // сума ряду
    int n = 0;        // номер члена ряду

    while (fabs(member) > accuracy)
    {
        n++;
        member = -member * argument * argument / ((2 * n - 1) * (2 * n));
        sum = sum + member;
    }

    return sum;
}

/*
 * Функція рахує y і стандартне значення для поточного x. Якщо x поза
 * проміжками або один зі знаменників майже нуль, функція не визначена.
 * При x = 1 формули перекриваються, діє перша
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void calculateFunction()
{
    float cosX = 0;                // cos(x) через ряд
    float denominator = 0;         // cos(x + 0.5) через ряд
    float standardDenominator = 0; // cos(x + 0.5) через бібліотечну функцію

    isDefined = true;
    if (x >= -1 - BORDER_ROUNDING && x <= 1 + BORDER_ROUNDING)
    {
        cosX = calculateCos(x);
        y = cosX * cosX * calculateCos(x * x);
        standardValue = cos(x) * cos(x) * cos(x * x);
    }
    else if (x > 1 && x <= 2 + BORDER_ROUNDING)
    {
        denominator = calculateCos(x + 0.5f);
        standardDenominator = cos(x + 0.5f);
        if (fabs(denominator) < MIN_DENOMINATOR
            || fabs(standardDenominator) < MIN_DENOMINATOR)
        {
            isDefined = false;
        }
        else
        {
            y = calculateCos(x / 2) / denominator;
            standardValue = cos(x / 2) / standardDenominator;
        }
    }
    else
    {
        isDefined = false;
    }
}

/*
 * Функція виводить таблицю. Значення x рахується через номер кроку,
 * щоб похибка float не накопичувалась і останній x не втрачався
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void printTable()
{
    stepCount = (int)((maxX - minX) / step + STEP_ROUNDING);

    cout << endl
         << setw(10) << "x" << setw(16) << "y" << setw(18) << "standard value"
         << setw(14) << "error" << endl;

    for (int i = 0; i <= stepCount; i++)
    {
        x = minX + i * step;
        calculateFunction();

        cout << fixed << setprecision(4) << setw(10) << x;
        if (isDefined)
        {
            deviation = fabs(y - standardValue);
            cout << setprecision(6) << setw(16) << y << setw(18)
                 << standardValue << scientific << setprecision(2) << setw(14)
                 << deviation;
        }
        else
        {
            cout << "    function is not defined";
        }
        cout << endl;
    }
}

int main()
{
    welcome();
    inputData();
    printTable();
    return 0;
}
