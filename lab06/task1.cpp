/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task1.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 1
// Запланована ціль: лабораторна робота №6, завдання 1, варіант 6.
//                   Знайти в цілочисельному масиві всі серії однакових
//                   елементів і довжину найдовшої серії.
/////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

//=========== константи
const int MAX_SIZE = 100;    // найбільша кількість елементів масиву
const int MIN_VALUE = -1000; // межі елемента, що вводиться з клавіатури
const int MAX_VALUE = 1000;
const int RANDOM_RANGE = 5; // випадкові числа від 0 до 4, щоб були повтори

//=========== глобальні змінні
int numbers[MAX_SIZE] = {0}; // масив
int elementCount = 0;        // кількість елементів масиву
int choice = 0;              // вибраний пункт меню
int seriesLength = 0;        // довжина поточної серії
int longestLength = 0;       // довжина найдовшої серії
int seriesCount = 0;         // кількість серій

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 6. Task 1. Variant 6." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;

    cout << "Find all series of equal elements standing in a row" << endl;
    cout << "and the length of the longest series." << endl << endl;
}

/*
 * Функція вводить ціле число. Якщо введено не число або число поза
 * межами, введення повторюється
 * Параметри: minValue, maxValue - межі допустимого значення
 * Значення, що повертаються: введене число
 */
int inputNumber(int minValue, int maxValue)
{
    int number = 0;

    cin >> number;
    while (cin.fail() || number < minValue || number > maxValue)
    {
        cin.clear();            // скинути помилку введення
        cin.ignore(1000, '\n'); // і пропустити неправильний рядок
        cout << "Error! Enter a number from " << minValue << " to " << maxValue
             << ": ";
        cin >> number;
    }

    return number;
}

/*
 * Функція вводить кількість елементів масиву
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputSize()
{
    cout << "Enter the number of elements (from 1 to " << MAX_SIZE << "): ";
    elementCount = inputNumber(1, MAX_SIZE);
}

/*
 * Функція вводить елементи масиву з клавіатури
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputArray()
{
    for (int i = 0; i < elementCount; i++)
    {
        cout << "Enter element " << i << ": ";
        numbers[i] = inputNumber(MIN_VALUE, MAX_VALUE);
    }
}

/*
 * Функція заповнює масив псевдовипадковими числами від 0 до 4
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void generateArray()
{
    srand(time(NULL));
    for (int i = 0; i < elementCount; i++)
    {
        numbers[i] = rand() % RANDOM_RANGE;
    }
}

/*
 * Функція виводить меню і створює масив вибраним способом
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void runMenu()
{
    cout << endl << "How to create the array:" << endl;
    cout << "1 - enter from the keyboard" << endl;
    cout << "2 - fill with pseudorandom numbers" << endl;
    cout << "Your choice: ";
    choice = inputNumber(1, 2);

    switch (choice)
    {
        case 1 : inputArray(); break;
        case 2 : generateArray(); break;
        default : cout << "There is no such menu item." << endl; break;
    }
}

/*
 * Функція виводить масив
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void printArray()
{
    cout << endl << "Array:";
    for (int i = 0; i < elementCount; i++)
    {
        cout << " " << numbers[i];
    }
    cout << endl;
}

/*
 * Функція виводить одну серію, що закінчується перед індексом endIndex
 * Параметри: endIndex - індекс елемента після серії
 * Значення, що повертаються: немає
 */
void printSeries(int endIndex)
{
    cout << "Series " << seriesCount << ":";
    for (int i = endIndex - seriesLength; i < endIndex; i++)
    {
        cout << " " << numbers[i];
    }
    cout << " (length " << seriesLength << ", starts at index "
         << endIndex - seriesLength << ")" << endl;
}

/*
 * Функція проходить масив, виводить усі серії і довжину найдовшої.
 * Серія закінчується, коли наступний елемент інший або масив закінчився
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void findSeries()
{
    cout << endl << "Series of equal elements:" << endl;

    seriesLength = 1;
    longestLength = 1;
    seriesCount = 0;

    for (int i = 1; i <= elementCount; i++)
    {
        if (i < elementCount && numbers[i] == numbers[i - 1])
        {
            seriesLength++;
        }
        else
        {
            seriesCount++;
            printSeries(i);
            if (seriesLength > longestLength)
            {
                longestLength = seriesLength;
            }
            seriesLength = 1;
        }
    }

    cout << endl << "Number of series: " << seriesCount << endl;
    cout << "Length of the longest series: " << longestLength << endl;
}

int main()
{
    welcome();
    inputSize();
    runMenu();
    printArray();
    findSeries();
    return 0;
}
