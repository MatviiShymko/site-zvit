/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task1.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 1
// Запланована ціль: лабораторна робота №7, завдання 1, варіант 6.
//                   Знайти максимальний і мінімальний елементи квадратної
//                   матриці, поміняти місцями їхні рядки, обчислити суму.
/////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
using namespace std;

//=========== константи
const int MIN_SIZE = 3;      // найменша розмірність матриці
const int MAX_SIZE = 10;     // найбільша розмірність матриці
const int MIN_VALUE = -1000; // межі елемента, що вводиться з клавіатури
const int MAX_VALUE = 1000;
const int RANDOM_LIMIT = 99; // випадкові числа від -99 до 99
const int CELL_WIDTH = 7;    // ширина стовпчика під час виведення

//=========== глобальні змінні
int matrix[MAX_SIZE][MAX_SIZE] = {{0}}; // квадратна матриця
int matrixSize = 0;                     // розмірність матриці
int choice = 0;                         // вибраний пункт меню
int maximum = 0;                        // максимальний елемент
int maxRow = 0;                         // його рядок
int maxColumn = 0;                      // його стовпчик
int minimum = 0;                        // мінімальний елемент
int minRow = 0;                         // його рядок
int minColumn = 0;                      // його стовпчик
int sum = 0;                            // шукана сума

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 7. Task 1. Variant 6." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;

    cout << "Find the maximum and minimum of a square matrix and swap their"
         << endl;
    cout << "rows. If they are in one column, find the sum of this column,"
         << endl;
    cout << "otherwise the sum of the elements above the main diagonal." << endl
         << endl;
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
 * Функція вводить розмірність матриці
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputSize()
{
    cout << "Enter the matrix size n (from " << MIN_SIZE << " to " << MAX_SIZE
         << "): ";
    matrixSize = inputNumber(MIN_SIZE, MAX_SIZE);
}

/*
 * Функція вводить елементи матриці з клавіатури
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputMatrix()
{
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            cout << "Enter the element [" << i << "][" << j << "]: ";
            matrix[i][j] = inputNumber(MIN_VALUE, MAX_VALUE);
        }
    }
}

/*
 * Функція заповнює матрицю псевдовипадковими числами від -99 до 99
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void generateMatrix()
{
    srand(time(NULL));
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            matrix[i][j] = rand() % (2 * RANDOM_LIMIT + 1) - RANDOM_LIMIT;
        }
    }
}

/*
 * Функція виводить меню і створює матрицю вибраним способом
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void runMenu()
{
    cout << endl << "How to create the matrix:" << endl;
    cout << "1 - enter from the keyboard" << endl;
    cout << "2 - fill with pseudorandom numbers" << endl;
    cout << "Your choice: ";
    choice = inputNumber(1, 2);

    switch (choice)
    {
        case 1 : inputMatrix(); break;
        case 2 : generateMatrix(); break;
        default : cout << "There is no such menu item." << endl; break;
    }
}

/*
 * Функція виводить матрицю у вигляді таблиці
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void printMatrix()
{
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            cout << setw(CELL_WIDTH) << matrix[i][j];
        }
        cout << endl;
    }
}

/*
 * Функція виводить вхідну матрицю, знаходить максимальний і мінімальний
 * елементи та їх індекси. Якщо таких елементів кілька, береться перший
 * під час проходу по рядках
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void findExtremes()
{
    cout << endl << "Input matrix:" << endl;
    printMatrix();

    maximum = matrix[0][0];
    minimum = matrix[0][0];

    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            if (matrix[i][j] > maximum)
            {
                maximum = matrix[i][j];
                maxRow = i;
                maxColumn = j;
            }
            if (matrix[i][j] < minimum)
            {
                minimum = matrix[i][j];
                minRow = i;
                minColumn = j;
            }
        }
    }

    cout << endl
         << "Maximum = " << maximum << ", row " << maxRow << ", column "
         << maxColumn << endl;
    cout << "Minimum = " << minimum << ", row " << minRow << ", column "
         << minColumn << endl;
}

/*
 * Функція міняє місцями рядки з максимальним і мінімальним елементами.
 * Якщо це один рядок, матриця не змінюється
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void swapRows()
{
    int temporary = 0;

    if (maxRow == minRow)
    {
        cout << endl
             << "Maximum and minimum are in the same row, "
             << "there is nothing to swap." << endl;
    }
    else
    {
        for (int j = 0; j < matrixSize; j++)
        {
            temporary = matrix[maxRow][j];
            matrix[maxRow][j] = matrix[minRow][j];
            matrix[minRow][j] = temporary;
        }
        cout << endl
             << "Rows " << maxRow << " and " << minRow << " are swapped."
             << endl;
    }

    cout << endl << "Output matrix:" << endl;
    printMatrix();
}

/*
 * Функція рахує суму для матриці після перестановки: суму стовпчика, якщо
 * максимальний і мінімальний елементи в ньому обидва, інакше суму
 * елементів над головною діагоналлю
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void calculateSum()
{
    sum = 0;

    if (maxColumn == minColumn)
    {
        for (int i = 0; i < matrixSize; i++)
        {
            sum = sum + matrix[i][maxColumn];
        }
        cout << endl << "Maximum and minimum are in the same column." << endl;
        cout << "Sum of column " << maxColumn << " = " << sum << endl;
    }
    else
    {
        for (int i = 0; i < matrixSize; i++)
        {
            for (int j = i + 1; j < matrixSize; j++)
            {
                sum = sum + matrix[i][j];
            }
        }
        cout << endl << "Maximum and minimum are in different columns." << endl;
        cout << "Sum of the elements above the main diagonal = " << sum << endl;
    }
}

int main()
{
    welcome();
    inputSize();
    runMenu();
    findExtremes();
    swapRows();
    calculateSum();
    return 0;
}
