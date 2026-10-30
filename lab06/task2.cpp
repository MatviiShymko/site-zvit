/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task2.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 1
// Запланована ціль: лабораторна робота №6, завдання 2, варіант 6.
//                   Парно-непарне сортування масиву, циклічний зсув
//                   і пошук ключа у відсортованому масиві з обертанням.
/////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

//=========== константи
const int MAX_SIZE = 100;    // найбільша кількість елементів масиву
const int MIN_VALUE = -1000; // межі елемента, що вводиться з клавіатури
const int MAX_VALUE = 1000;
const int RANDOM_RANGE = 100; // випадкові числа від 0 до 99

//=========== глобальні змінні
int numbers[MAX_SIZE] = {0};        // масив
int shiftedNumbers[MAX_SIZE] = {0}; // допоміжний масив для зсуву
int elementCount = 0;               // кількість елементів масиву
int choice = 0;                     // вибраний пункт меню
bool isDetailed = false;            // чи виводити проміжні ітерації
bool isSorted = false;              // чи не було обмінів за ітерацію
int iteration = 0;                  // номер ітерації сортування
int comparisonCount = 0;            // кількість порівнянь у сортуванні
int swapCount = 0;                  // кількість обмінів у сортуванні
int shift = 0;                      // зсув вправо
int key = 0;                        // шукане значення
int leftIndex = 0;                  // ліва межа відрізка пошуку
int rightIndex = 0;                 // права межа відрізка пошуку
int middleIndex = 0;                // середина відрізка пошуку
int foundIndex = -1;                // індекс ключа, -1 якщо його немає
int searchCount = 0;                // кількість порівнянь із ключем

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 6. Task 2. Variant 6." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;

    cout << "Sort the array by even-odd sort, shift it cyclically" << endl;
    cout << "and search for a key in the rotated sorted array." << endl << endl;
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
 * Функція заповнює масив псевдовипадковими числами від 0 до 99
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
 * Функція вводить режим виведення результатів
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputOutputMode()
{
    cout << endl << "Output mode:" << endl;
    cout << "1 - only the sorted array" << endl;
    cout << "2 - with intermediate iterations" << endl;
    cout << "Your choice: ";
    choice = inputNumber(1, 2);
    if (choice == 2)
    {
        isDetailed = true;
    }
}

/*
 * Функція виводить елементи масиву в один рядок
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void printArray()
{
    for (int i = 0; i < elementCount; i++)
    {
        cout << " " << numbers[i];
    }
    cout << endl;
}

/*
 * Функція порівнює пари сусідніх елементів, починаючи з firstIndex,
 * і міняє місцями ті, що стоять неправильно
 * Параметри: firstIndex - індекс першого елемента першої пари
 * Значення, що повертаються: немає
 */
void sortPairs(int firstIndex)
{
    int temporary = 0;

    for (int i = firstIndex; i < elementCount - 1; i = i + 2)
    {
        comparisonCount++;
        if (numbers[i] > numbers[i + 1])
        {
            temporary = numbers[i];
            numbers[i] = numbers[i + 1];
            numbers[i + 1] = temporary;
            swapCount++;
            isSorted = false;
        }
    }
}

/*
 * Функція сортує масив за зростанням. Кожна ітерація проходить пари
 * з парним першим індексом, потім з непарним. Сортування закінчується,
 * коли за ітерацію не було жодного обміну
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void sortArray()
{
    cout << endl << "Input array:";
    printArray();

    isSorted = false;
    while (!isSorted)
    {
        isSorted = true;
        iteration++;

        sortPairs(0);
        if (isDetailed)
        {
            cout << "Iteration " << iteration << ", even pairs:";
            printArray();
        }

        sortPairs(1);
        if (isDetailed)
        {
            cout << "Iteration " << iteration << ", odd pairs: ";
            printArray();
        }
    }

    cout << "Sorted array:";
    printArray();
    cout << "Iterations: " << iteration << endl;
    cout << "Comparisons: " << comparisonCount << endl;
    cout << "Swaps: " << swapCount << endl;
}

/*
 * Функція вводить зсув і циклічно зсуває масив вправо
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void rotateArray()
{
    cout << endl
         << "Enter the cyclic shift to the right (from 0 to "
         << elementCount - 1 << "): ";
    shift = inputNumber(0, elementCount - 1);

    for (int i = 0; i < elementCount; i++)
    {
        shiftedNumbers[(i + shift) % elementCount] = numbers[i];
    }

    for (int i = 0; i < elementCount; i++)
    {
        numbers[i] = shiftedNumbers[i];
    }

    cout << "Rotated array:";
    printArray();
}

/*
 * Функція шукає ключ у масиві з обертанням. На кожному кроці одна
 * половина відрізка відсортована: якщо ключ лежить у ній, пошук іде
 * там, інакше в другій половині. Якщо обидва краї дорівнюють
 * середньому елементу, половину визначити не можна, тому краї
 * відкидаються
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void searchKey()
{
    cout << endl << "Enter the key: ";
    key = inputNumber(MIN_VALUE, MAX_VALUE);

    leftIndex = 0;
    rightIndex = elementCount - 1;
    foundIndex = -1;

    while (leftIndex <= rightIndex && foundIndex == -1)
    {
        middleIndex = (leftIndex + rightIndex) / 2;
        searchCount++;

        if (isDetailed)
        {
            cout << "Step " << searchCount << ": left = " << leftIndex
                 << ", right = " << rightIndex << ", middle = " << middleIndex
                 << ", element = " << numbers[middleIndex] << endl;
        }

        if (numbers[middleIndex] == key)
        {
            foundIndex = middleIndex;
        }
        else if (numbers[leftIndex] == numbers[middleIndex]
                 && numbers[rightIndex] == numbers[middleIndex])
        {
            leftIndex++;
            rightIndex--;
        }
        else if (numbers[leftIndex] <= numbers[middleIndex])
        {
            if (numbers[leftIndex] <= key && key < numbers[middleIndex])
            {
                rightIndex = middleIndex - 1;
            }
            else
            {
                leftIndex = middleIndex + 1;
            }
        }
        else
        {
            if (numbers[middleIndex] < key && key <= numbers[rightIndex])
            {
                leftIndex = middleIndex + 1;
            }
            else
            {
                rightIndex = middleIndex - 1;
            }
        }
    }

    if (foundIndex == -1)
    {
        cout << "The key " << key << " is not in the array." << endl;
    }
    else
    {
        cout << "The key " << key << " is at index " << foundIndex << "."
             << endl;
    }
    cout << "Search comparisons: " << searchCount << endl;
}

int main()
{
    welcome();
    inputSize();
    runMenu();
    inputOutputMode();
    sortArray();
    rotateArray();
    searchKey();
    return 0;
}
