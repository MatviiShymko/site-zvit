/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task2.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 1
// Запланована ціль: лабораторна робота №7, завдання 2, варіант 6.
//                   Знайти суму і різницю двох матриць однакової
//                   вимірності, перевірити матриці на рівність.
/////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
using namespace std;

//=========== константи
const int MAX_SIZE = 10;     // найбільша кількість рядків і стовпчиків
const int MIN_VALUE = -1000; // межі елемента матриці
const int MAX_VALUE = 1000;
const int CELL_WIDTH = 7; // ширина стовпчика під час виведення

//=========== глобальні змінні
int matrixA[MAX_SIZE][MAX_SIZE] = {{0}};          // матриця A
int matrixB[MAX_SIZE][MAX_SIZE] = {{0}};          // матриця B
int matrixSum[MAX_SIZE][MAX_SIZE] = {{0}};        // сума A + B
int matrixDifference[MAX_SIZE][MAX_SIZE] = {{0}}; // різниця A - B
int rowsA = 0;                                    // вимірність матриці A
int columnsA = 0;
int rowsB = 0; // вимірність матриці B
int columnsB = 0;
int choice = 0;          // вибраний пункт меню
bool isDetailed = false; // чи виводити проміжні ітерації
int lowerBound = 0;      // межі випадкових чисел
int upperBound = 0;
int differentCount = 0; // скільки елементів не збігаються

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 7. Task 2. Variant 6." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;

    cout << "Find the sum and the difference of matrices A and B" << endl;
    cout << "and check whether they are equal." << endl << endl;
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
 * Функція вводить вимірності обох матриць
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputSizes()
{
    cout << "Enter the rows of A (from 1 to " << MAX_SIZE << "): ";
    rowsA = inputNumber(1, MAX_SIZE);
    cout << "Enter the columns of A (from 1 to " << MAX_SIZE << "): ";
    columnsA = inputNumber(1, MAX_SIZE);
    cout << "Enter the rows of B (from 1 to " << MAX_SIZE << "): ";
    rowsB = inputNumber(1, MAX_SIZE);
    cout << "Enter the columns of B (from 1 to " << MAX_SIZE << "): ";
    columnsB = inputNumber(1, MAX_SIZE);
}

/*
 * Функція вводить елементи матриці з клавіатури
 * Параметри: matrix - матриця, name - її назва для підказки
 * Значення, що повертаються: немає
 */
void inputMatrix(int matrix[MAX_SIZE][MAX_SIZE], char name)
{
    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < columnsA; j++)
        {
            cout << "Enter " << name << "[" << i << "][" << j << "]: ";
            matrix[i][j] = inputNumber(MIN_VALUE, MAX_VALUE);
        }
    }
}

/*
 * Функція вводить діапазон псевдовипадкових чисел
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputRange()
{
    srand(time(NULL)); // запустити генератор один раз, перед генерацією

    cout << "Enter the lower bound (from " << MIN_VALUE << " to " << MAX_VALUE
         << "): ";
    lowerBound = inputNumber(MIN_VALUE, MAX_VALUE);
    cout << "Enter the upper bound (from " << lowerBound << " to " << MAX_VALUE
         << "): ";
    upperBound = inputNumber(lowerBound, MAX_VALUE);
}

/*
 * Функція заповнює матрицю псевдовипадковими числами з діапазону
 * Параметри: matrix - матриця, яку заповнюємо
 * Значення, що повертаються: немає
 */
void generateMatrix(int matrix[MAX_SIZE][MAX_SIZE])
{
    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < columnsA; j++)
        {
            matrix[i][j] = rand() % (upperBound - lowerBound + 1) + lowerBound;
        }
    }
}

/*
 * Функція виводить меню і створює матриці вибраним способом
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void runMenu()
{
    cout << endl << "How to create the matrices:" << endl;
    cout << "1 - enter from the keyboard" << endl;
    cout << "2 - fill with pseudorandom numbers in a given range" << endl;
    cout << "Your choice: ";
    choice = inputNumber(1, 2);

    switch (choice)
    {
        case 1 :
            inputMatrix(matrixA, 'A');
            inputMatrix(matrixB, 'B');
            break;
        case 2 :
            inputRange();
            generateMatrix(matrixA);
            generateMatrix(matrixB);
            break;
        default : cout << "There is no such menu item." << endl; break;
    }
}

/*
 * Функція вводить режим виведення результатів
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputOutputMode()
{
    cout << endl << "Output mode:" << endl;
    cout << "1 - only the final result" << endl;
    cout << "2 - with intermediate iterations" << endl;
    cout << "Your choice: ";
    choice = inputNumber(1, 2);
    if (choice == 2)
    {
        isDetailed = true;
    }
}

/*
 * Функція виводить матрицю у вигляді таблиці
 * Параметри: matrix - матриця, яку виводимо
 * Значення, що повертаються: немає
 */
void printMatrix(int matrix[MAX_SIZE][MAX_SIZE])
{
    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < columnsA; j++)
        {
            cout << setw(CELL_WIDTH) << matrix[i][j];
        }
        cout << endl;
    }
}

/*
 * Функція обчислює суму матриць
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void calculateSum()
{
    cout << endl;
    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < columnsA; j++)
        {
            matrixSum[i][j] = matrixA[i][j] + matrixB[i][j];
            if (isDetailed)
            {
                cout << "S[" << i << "][" << j << "] = " << matrixA[i][j]
                     << " + " << matrixB[i][j] << " = " << matrixSum[i][j]
                     << endl;
            }
        }
    }

    cout << "Sum A + B:" << endl;
    printMatrix(matrixSum);
}

/*
 * Функція обчислює різницю матриць
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void calculateDifference()
{
    cout << endl;
    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < columnsA; j++)
        {
            matrixDifference[i][j] = matrixA[i][j] - matrixB[i][j];
            if (isDetailed)
            {
                cout << "D[" << i << "][" << j << "] = " << matrixA[i][j]
                     << " - " << matrixB[i][j] << " = "
                     << matrixDifference[i][j] << endl;
            }
        }
    }

    cout << "Difference A - B:" << endl;
    printMatrix(matrixDifference);
}

/*
 * Функція порівнює матриці поелементно. Матриці рівні, якщо немає
 * жодної пари різних елементів
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void compareMatrices()
{
    cout << endl;
    differentCount = 0;

    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < columnsA; j++)
        {
            if (matrixA[i][j] != matrixB[i][j])
            {
                differentCount++;
            }
            if (isDetailed)
            {
                cout << "A[" << i << "][" << j << "] = " << matrixA[i][j]
                     << ", B[" << i << "][" << j << "] = " << matrixB[i][j]
                     << ", different so far: " << differentCount << endl;
            }
        }
    }

    if (differentCount == 0)
    {
        cout << "The matrices are equal." << endl;
    }
    else
    {
        cout << "The matrices are not equal, different elements: "
             << differentCount << endl;
    }
}

/*
 * Функція виконує операції, якщо вимірності однакові, інакше
 * виводить повідомлення
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void processMatrices()
{
    if (rowsA != rowsB || columnsA != columnsB)
    {
        cout << endl
             << "The dimensions are different: " << rowsA << " x " << columnsA
             << " and " << rowsB << " x " << columnsB << "." << endl;
        cout << "The sum and the difference cannot be found, "
             << "and the matrices are not equal." << endl;
    }
    else
    {
        runMenu();
        inputOutputMode();

        cout << endl << "Matrix A:" << endl;
        printMatrix(matrixA);
        cout << "Matrix B:" << endl;
        printMatrix(matrixB);

        calculateSum();
        calculateDifference();
        compareMatrices();
    }
}

int main()
{
    welcome();
    inputSizes();
    processMatrices();
    return 0;
}
