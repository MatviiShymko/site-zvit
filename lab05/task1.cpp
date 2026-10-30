/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task1.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 1
// Запланована ціль: лабораторна робота №5, завдання 1, варіант 10.
//                   Надрукувати трикутник Паскаля рекурсивно й
//                   ітераційно, порівняти ефективність.
/////////////////////////////////////////////////////////////////////

#include <iomanip>
#include <iostream>
using namespace std;

//=========== константи
const int MIN_ROWS = 1;      // найменша кількість рядків
const int MAX_ROWS = 15;     // найбільша кількість рядків
const int ELEMENT_WIDTH = 5; // ширина одного числа в трикутнику

//=========== глобальні змінні
int rowCount = 0;       // кількість рядків трикутника
int callCount = 0;      // кількість рекурсивних викликів
int recursionDepth = 0; // найбільша глибина рекурсії
int iterationCount = 0; // кількість ітерацій циклу

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 5. Task 1. Variant 10." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;

    cout << "Print the first n rows of Pascal's triangle in two ways:" << endl;
    cout << "with recursion and with a loop. Compare the efficiency." << endl
         << endl;
}

/*
 * Функція вводить кількість рядків. Якщо введено не число або число
 * поза межами, введення повторюється
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputRowCount()
{
    cout << "Enter n (from 1 to 15): ";
    cin >> rowCount;
    while (cin.fail() || rowCount < MIN_ROWS || rowCount > MAX_ROWS)
    {
        cin.clear();            // скинути помилку введення
        cin.ignore(1000, '\n'); // і пропустити неправильний рядок
        cout << "Error! Enter n from 1 to 15: ";
        cin >> rowCount;
    }
}

/*
 * Функція рекурсивно рахує елемент трикутника:
 * f(row, 0) = f(row, row) = 1,
 * f(row, column) = f(row - 1, column - 1) + f(row - 1, column).
 * Рахує виклики й запам'ятовує найбільшу глибину
 * Параметри: row - номер рядка, column - номер місця в рядку,
 *            depth - глибина поточного виклику
 * Значення, що повертаються: значення елемента
 */
int calculateElement(int row, int column, int depth)
{
    int element = 1; // на краях трикутника стоять одиниці

    callCount++;
    if (depth > recursionDepth)
    {
        recursionDepth = depth;
    }

    if (column != 0 && column != row)
    {
        element = calculateElement(row - 1, column - 1, depth + 1)
                  + calculateElement(row - 1, column, depth + 1);
    }

    return element;
}

/*
 * Функція друкує трикутник, викликаючи рекурсію для кожного елемента
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void printRecursive()
{
    cout << endl << "Recursive variant:" << endl;

    for (int i = 0; i < rowCount; i++)
    {
        cout << setw((rowCount - i - 1) * ELEMENT_WIDTH / 2) << "";
        for (int j = 0; j <= i; j++)
        {
            cout << setw(ELEMENT_WIDTH) << calculateElement(i, j, 1);
        }
        cout << endl;
    }
}

/*
 * Функція друкує трикутник у циклі. Кожен наступний елемент рядка
 * рахується через попередній: C(i, j) = C(i, j - 1) * (i - j + 1) / j
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void printIterative()
{
    int element = 1; // поточний елемент рядка

    cout << endl << "Iterative variant:" << endl;

    for (int i = 0; i < rowCount; i++)
    {
        cout << setw((rowCount - i - 1) * ELEMENT_WIDTH / 2) << "";

        element = 1;
        cout << setw(ELEMENT_WIDTH) << element;
        for (int j = 1; j <= i; j++)
        {
            element = element * (i - j + 1) / j;
            iterationCount++;
            cout << setw(ELEMENT_WIDTH) << element;
        }
        cout << endl;
    }
}

/*
 * Функція виводить показники ефективності обох варіантів
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void printEfficiency()
{
    cout << endl << "Efficiency for n = " << rowCount << ":" << endl;
    cout << "Recursive calls: " << callCount << endl;
    cout << "Recursion depth: " << recursionDepth << endl;
    cout << "Loop iterations: " << iterationCount << endl;
}

int main()
{
    welcome();
    inputRowCount();
    printRecursive();
    printIterative();
    printEfficiency();
    return 0;
}
