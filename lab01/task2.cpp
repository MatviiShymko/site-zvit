/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task2.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 2
// Запланована ціль: лабораторна робота №1, завдання 2, варіант 5.
//                   Обчислити відстань між двома точками площини.
/////////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>
using namespace std;

//=========== глобальні змінні
float firstX = 0;      // координата x0 першої точки
float firstY = 0;      // координата y0 першої точки
float secondX = 0;     // координата x1 другої точки
float secondY = 0;     // координата y1 другої точки
float differenceX = 0; // різниця координат по осі Ox
float differenceY = 0; // різниця координат по осі Oy
float length = 0;      // відстань між точками

/*
 * Функція вводить координати двох точок і виводить відстань між ними
 * Параметри: немає
 * Значення, що повертаються: 0
 */
int main()
{
    cout << "Lab work 1. Task 2. Variant 5." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;
    cout << "Calculate the distance between points (x0, y0) and (x1, y1)"
         << endl
         << endl;

    cout << "Enter x0 and y0: ";
    cin >> firstX >> firstY;
    while (cin.fail())
    {
        cin.clear();            // скинути помилку введення
        cin.ignore(1000, '\n'); // і пропустити неправильний рядок
        cout << "Error! Enter two numbers: ";
        cin >> firstX >> firstY;
    }

    cout << "Enter x1 and y1: ";
    cin >> secondX >> secondY;
    while (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Error! Enter two numbers: ";
        cin >> secondX >> secondY;
    }

    differenceX = secondX - firstX;
    differenceY = secondY - firstY;
    length = sqrt(differenceX * differenceX + differenceY * differenceY);

    cout << "Distance = " << length << endl;

    return 0;
}
