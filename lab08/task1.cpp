/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task1.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 1
// Запланована ціль: лабораторна робота №8, варіант 6.
//                   Зберегти розріджену матрицю у форматі CRS, знайти
//                   скалярні добутки рядків двома алгоритмами
//                   і порівняти час та обсяг пам'яті.
/////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
using namespace std;

//=========== константи
const int MAX_SIZE = 1000;            // найбільша кількість рядків і стовпців
const int MAX_VALUE = 9;              // найбільший модуль елемента матриці
const int ZERO_LIMIT = 60;            // нулів має бути більше за цей відсоток
const int COMMAND_COUNT = 7;          // кількість команд меню без виходу
const int CREATE_COMMANDS = 2;        // команд, що створюють матрицю
const int OPERATION_LIMIT = 20000000; // для підбору кількості повторів

//=========== глобальні змінні
int** matrix = NULL;       // покажчик на матрицю
int* values = NULL;        // масив AN: ненульові елементи
int* columnNumbers = NULL; // масив JA: номери стовпців цих елементів
int* rowStarts = NULL;     // масив IA: початки рядків у масиві AN
int rowCount = 0;          // кількість рядків
int columnCount = 0;       // кількість стовпців
int nonZeroCount = 0;      // кількість ненульових елементів NNZ
int choice = 0;            // номер команди меню
void (*command)() = NULL;  // покажчик на функцію вибраної команди

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 8. Variant 6." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;

    cout << "Sparse matrix in CRS format: scalar products of rows 1 and 2,"
         << endl;
    cout << "3 and 4, and so on by the sparse and the standard algorithm."
         << endl;
}

/*
 * Функція вводить ціле число. Якщо введено не число або число
 * поза межами, введення повторюється
 * Параметри: minValue, maxValue - межі числа
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
 * Функція вводить розміри матриці. Кількість рядків має бути парною
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputSize()
{
    cout << "Enter an even number of rows (from 2 to " << MAX_SIZE << "): ";
    rowCount = inputNumber(2, MAX_SIZE);
    while (rowCount % 2 != 0)
    {
        cout << "Error! The number of rows must be even. Enter it again: ";
        rowCount = inputNumber(2, MAX_SIZE);
    }

    cout << "Enter the number of columns (from 1 to " << MAX_SIZE << "): ";
    columnCount = inputNumber(1, MAX_SIZE);
}

/*
 * Функція виділяє пам'ять під матрицю і заповнює її нулями
 * Параметри: rows, columns - розміри матриці
 * Значення, що повертаються: покажчик на створену матрицю
 */
int** createMatrix(int rows, int columns)
{
    int** table = new int*[rows];

    for (int i = 0; i < rows; i++)
    {
        *(table + i) = new int[columns];
        for (int j = 0; j < columns; j++)
        {
            *(*(table + i) + j) = 0;
        }
    }

    return table;
}

/*
 * Функція виділяє масиви AN, JA, IA і заповнює їх за матрицею
 * Параметри: table - покажчик на матрицю
 * Значення, що повертаються: немає
 */
void convertToCrs(int** table)
{
    int position = 0; // номер наступного вільного місця в AN і JA

    values = new int[nonZeroCount];
    columnNumbers = new int[nonZeroCount];
    rowStarts = new int[rowCount + 1];

    for (int i = 0; i < rowCount; i++)
    {
        *(rowStarts + i) = position;
        for (int j = 0; j < columnCount; j++)
        {
            if (*(*(table + i) + j) != 0)
            {
                *(values + position) = *(*(table + i) + j);
                *(columnNumbers + position) = j;
                position++;
            }
        }
    }

    *(rowStarts + rowCount) = position; // кінцевий елемент IA дорівнює NNZ
}

/*
 * Функція звільняє всю динамічну пам'ять матриці й масивів CRS
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void freeMemory()
{
    if (matrix != NULL)
    {
        for (int i = 0; i < rowCount; i++)
        {
            delete[] *(matrix + i);
        }

        delete[] matrix;
        delete[] values;
        delete[] columnNumbers;
        delete[] rowStarts;

        // щоб наступний виклик не звільнив ті самі масиви вдруге
        matrix = NULL;
        values = NULL;
        columnNumbers = NULL;
        rowStarts = NULL;
    }
}

/*
 * Команда меню: генерує матрицю із заданим відсотком нулів. Ненульові
 * значення ставляться у випадкові клітинки, зайняті обходяться
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void generateMatrix()
{
    int row = 0;
    int column = 0;
    int placed = 0; // скільки ненульових елементів уже поставлено
    int zeroPercent = 0;

    freeMemory();
    inputSize();
    cout << "Enter the percent of zeros (from " << ZERO_LIMIT + 1
         << " to 99): ";
    zeroPercent = inputNumber(ZERO_LIMIT + 1, 99);
    nonZeroCount = rowCount * columnCount * (100 - zeroPercent) / 100;

    matrix = createMatrix(rowCount, columnCount);
    while (placed < nonZeroCount)
    {
        row = rand() % rowCount;
        column = rand() % columnCount;
        if (*(*(matrix + row) + column) == 0)
        {
            // значення від -MAX_VALUE до MAX_VALUE, нуль просто не рахується
            *(*(matrix + row) + column) =
                rand() % (2 * MAX_VALUE + 1) - MAX_VALUE;
            if (*(*(matrix + row) + column) != 0)
            {
                placed++;
            }
        }
    }

    convertToCrs(matrix);
    cout << "Matrix is created, NNZ = " << nonZeroCount << endl;
}

/*
 * Команда меню: вводить матрицю з клавіатури, щоб перевірити програму
 * на прикладі, порахованому вручну
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void enterMatrix()
{
    freeMemory();
    inputSize();
    matrix = createMatrix(rowCount, columnCount);
    nonZeroCount = 0;

    for (int i = 0; i < rowCount; i++)
    {
        cout << "Enter row " << i + 1 << " (" << columnCount << " numbers): ";
        for (int j = 0; j < columnCount; j++)
        {
            *(*(matrix + i) + j) = inputNumber(-MAX_VALUE, MAX_VALUE);
            if (*(*(matrix + i) + j) != 0)
            {
                nonZeroCount++;
            }
        }
    }

    if (nonZeroCount * 100 < rowCount * columnCount * (100 - ZERO_LIMIT))
    {
        convertToCrs(matrix);
        cout << "Matrix is stored, NNZ = " << nonZeroCount << endl;
    }
    else
    {
        cout << "Error! Zeros must be more than " << ZERO_LIMIT << "%." << endl;
        freeMemory();
    }
}

/*
 * Команда меню: виводить матрицю по рядках та стовпцях
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void showMatrix()
{
    for (int i = 0; i < rowCount; i++)
    {
        for (int j = 0; j < columnCount; j++)
        {
            cout << setw(3) << *(*(matrix + i) + j);
        }
        cout << endl;
    }
}

/*
 * Функція виводить одновимірний масив в один рядок
 * Параметри: array - покажчик на масив, size - кількість елементів
 * Значення, що повертаються: немає
 */
void printArray(int* array, int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << *(array + i) << " ";
    }
    cout << endl;
}

/*
 * Команда меню: виводить масиви AN, JA, IA
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void showArrays()
{
    cout << "AN: ";
    printArray(values, nonZeroCount);
    cout << "JA: ";
    printArray(columnNumbers, nonZeroCount);
    cout << "IA: ";
    printArray(rowStarts, rowCount + 1);
}

/*
 * Функція рахує добутки пар рядків за масивами CRS. Стовпці в рядку йдуть
 * за зростанням, тому менший номер стовпця просувається вперед, а
 * множаться лише елементи зі спільного стовпця
 * Параметри: elements - покажчик на AN, columns - покажчик на JA,
 *            starts - покажчик на IA, rows - кількість рядків
 * Значення, що повертаються: покажчик на масив добутків
 */
int* calculateSparseProducts(int* elements, int* columns, int* starts, int rows)
{
    int* result = new int[rows / 2];

    for (int i = 0; i < rows; i = i + 2)
    {
        int first = *(starts + i);      // поточний елемент першого рядка
        int second = *(starts + i + 1); // поточний елемент другого рядка
        int sum = 0;

        while (first < *(starts + i + 1) && second < *(starts + i + 2))
        {
            if (*(columns + first) == *(columns + second))
            {
                sum = sum + *(elements + first) * *(elements + second);
                first++;
                second++;
            }
            else if (*(columns + first) < *(columns + second))
            {
                first++;
            }
            else
            {
                second++;
            }
        }

        *(result + i / 2) = sum;
    }

    return result;
}

/*
 * Функція рахує добутки пар рядків за повною матрицею
 * Параметри: table - покажчик на матрицю, rows, columns - її розміри
 * Значення, що повертаються: покажчик на масив добутків
 */
int* calculateStandardProducts(int** table, int rows, int columns)
{
    int* result = new int[rows / 2];

    for (int i = 0; i < rows; i = i + 2)
    {
        *(result + i / 2) = 0;
        for (int j = 0; j < columns; j++)
        {
            *(result + i / 2) = *(result + i / 2)
                                + *(*(table + i) + j) * *(*(table + i + 1) + j);
        }
    }

    return result;
}

/*
 * Функція виводить таблицю добутків і звільняє пам'ять цього масиву
 * Параметри: result - покажчик на масив добутків
 * Значення, що повертаються: немає
 */
void printProducts(int* result)
{
    cout << setw(10) << "rows" << setw(10) << "product" << endl;
    for (int i = 0; i < rowCount / 2; i++)
    {
        cout << setw(5) << 2 * i + 1 << setw(5) << 2 * i + 2 << setw(10)
             << *(result + i) << endl;
    }

    delete[] result;
}

/*
 * Команда меню: добутки за алгоритмом для розрідженої матриці
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void showSparseProducts()
{
    cout << "Sparse algorithm:" << endl;
    printProducts(
        calculateSparseProducts(values, columnNumbers, rowStarts, rowCount));
}

/*
 * Команда меню: добутки за стандартним алгоритмом
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void showStandardProducts()
{
    cout << "Standard algorithm:" << endl;
    printProducts(calculateStandardProducts(matrix, rowCount, columnCount));
}

/*
 * Команда меню: порівнює час і пам'ять двох алгоритмів. Одне обчислення
 * надто швидке для clock(), тому воно повторюється багато разів
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void compareAlgorithms()
{
    int repeatCount = OPERATION_LIMIT / (rowCount * columnCount) + 1;
    int crsMemory = (2 * nonZeroCount + rowCount + 1) * sizeof(int);
    int fullMemory =
        rowCount * sizeof(int*) + rowCount * columnCount * sizeof(int);
    float sparseTime = 0; // час усіх повторів, мс
    float standardTime = 0;
    clock_t startTime = clock();

    for (int i = 0; i < repeatCount; i++)
    {
        delete[] calculateSparseProducts(values, columnNumbers, rowStarts,
                                         rowCount);
    }
    sparseTime = (float)(clock() - startTime) * 1000 / CLOCKS_PER_SEC;

    startTime = clock();
    for (int i = 0; i < repeatCount; i++)
    {
        delete[] calculateStandardProducts(matrix, rowCount, columnCount);
    }
    standardTime = (float)(clock() - startTime) * 1000 / CLOCKS_PER_SEC;

    cout << "Repeats: " << repeatCount << endl << fixed << setprecision(3);
    cout << "Sparse:   " << sparseTime << " ms, " << crsMemory << " bytes"
         << endl;
    cout << "Standard: " << standardTime << " ms, " << fullMemory << " bytes"
         << endl;
    cout << "One run: sparse " << sparseTime * 1000 / repeatCount
         << " us, standard " << standardTime * 1000 / repeatCount << " us"
         << endl;

    if (sparseTime > 0)
    {
        cout << "Time ratio standard / sparse: " << standardTime / sparseTime
             << endl;
    }
    cout << "Memory ratio standard / sparse: " << (float)fullMemory / crsMemory
         << endl;
}

/*
 * Функція показує меню і викликає команди через покажчик на функцію
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void runMenu()
{
    void (*commands[COMMAND_COUNT])() = {
        generateMatrix,     enterMatrix,          showMatrix,       showArrays,
        showSparseProducts, showStandardProducts, compareAlgorithms};

    srand(time(NULL));

    do
    {
        cout << endl << "1 - generate a matrix    2 - enter a matrix" << endl;
        cout << "3 - print the matrix     4 - print arrays AN, JA, IA" << endl;
        cout << "5 - sparse products      6 - standard products" << endl;
        cout << "7 - compare algorithms   0 - exit" << endl;
        cout << "Enter a command: ";
        choice = inputNumber(0, COMMAND_COUNT);

        if (choice > CREATE_COMMANDS && matrix == NULL)
        {
            cout << "Create a matrix first (command 1 or 2)." << endl;
        }
        else if (choice != 0)
        {
            command = *(commands + choice - 1); // вибрати покажчик на функцію
            (*command)();                       // викликати функцію через нього
        }
    } while (choice != 0);
}

int main()
{
    welcome();
    runMenu();
    freeMemory();
    return 0;
}
