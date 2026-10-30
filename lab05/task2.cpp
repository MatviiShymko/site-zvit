/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task2.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 1
// Запланована ціль: лабораторна робота №5, завдання 2, варіант 10.
//                   Знайти послідовності з a нулів і b одиниць, де
//                   два нулі не стоять поруч. Без масивів і рядків.
/////////////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

//=========== константи
const int MAX_LENGTH = 9;     // більша довжина не вміститься в int
const int ROW_SIZE = 6;       // кількість послідовностей у рядку
const int START_SEQUENCE = 1; // одиниця-маркер на початку числа

//=========== глобальні змінні
int zeroCount = 0;     // кількість нулів a
int oneCount = 0;      // кількість одиниць b
int sequenceCount = 0; // кількість знайдених послідовностей

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 5. Task 2. Variant 10." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;

    cout << "Find all sequences of a zeros and b ones" << endl;
    cout << "where no two zeros stand next to each other." << endl << endl;
}

/*
 * Функція вводить ціле число. Якщо введено не число або число
 * поза межами, введення повторюється
 * Параметри: minValue - найменше допустиме значення,
 *            maxValue - найбільше допустиме значення
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
 * Функція вводить a і b, сума яких не більша за 9
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputData()
{
    cout << "Enter a (from 1 to 8): ";
    zeroCount = inputNumber(1, MAX_LENGTH - 1);

    cout << "Enter b (from 1 to " << MAX_LENGTH - zeroCount << "): ";
    oneCount = inputNumber(1, MAX_LENGTH - zeroCount);
}

/*
 * Функція рекурсивно друкує цифри послідовності без маркера:
 * спочатку всі цифри, крім останньої, потім останню
 * Параметри: sequence - послідовність, записана цифрами числа
 * Значення, що повертаються: немає
 */
void printSequence(int sequence)
{
    if (sequence != START_SEQUENCE)
    {
        printSequence(sequence / 10);
        cout << sequence % 10;
    }
}

/*
 * Функція рекурсивно будує послідовності, дописуючи по одній цифрі.
 * Нуль можна дописати, лише якщо попередня цифра не нуль
 * Параметри: zerosLeft, onesLeft - скільки цифр ще треба дописати,
 *            lastDigit - попередня цифра,
 *            sequence - вже побудована частина числом з маркером
 * Значення, що повертаються: немає
 */
void buildSequences(int zerosLeft, int onesLeft, int lastDigit, int sequence)
{
    if (zerosLeft == 0 && onesLeft == 0)
    {
        printSequence(sequence);
        cout << " ";

        sequenceCount++;
        if (sequenceCount % ROW_SIZE == 0)
        {
            cout << endl;
        }
    }
    else
    {
        if (zerosLeft > 0 && lastDigit != 0)
        {
            buildSequences(zerosLeft - 1, onesLeft, 0, sequence * 10);
        }
        if (onesLeft > 0)
        {
            buildSequences(zerosLeft, onesLeft - 1, 1, sequence * 10 + 1);
        }
    }
}

/*
 * Функція запускає пошук і виводить кількість послідовностей
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void printResult()
{
    cout << endl << "Sequences:" << endl;
    buildSequences(zeroCount, oneCount, 1, START_SEQUENCE);

    if (sequenceCount == 0)
    {
        cout << "no such sequences";
    }
    cout << endl << endl << "Number of sequences: " << sequenceCount << endl;
}

int main()
{
    welcome();
    inputData();
    printResult();
    return 0;
}
