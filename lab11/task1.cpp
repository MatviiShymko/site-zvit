/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task1.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 2
// Запланована ціль: лабораторна робота №11, завдання 1, варіант 6.1.
//                   Записати рядки з клавіатури у текстовий файл,
//                   визначити його характеристики і записати їх
//                   у новий текстовий файл.
/////////////////////////////////////////////////////////////////////

#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;

//=========== константи
const int MAX_LINES = 20;    // найбільша кількість рядків з клавіатури
const int LINE_LENGTH = 256; // розмір рядка разом із символом кінця рядка
const char TEXT_FILE[] = "lines.txt";    // текстовий файл
const char RESULT_FILE[] = "result.txt"; // новий файл результатів

//=========== глобальні змінні
ifstream inputFile;          // потік для читання файлу
ofstream outputFile;         // потік для запису файлу
char line[LINE_LENGTH] = ""; // поточний рядок

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 11. Task 1. Variant 6.1." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;

    cout << "Write lines from the keyboard to a text file. Find the number of"
         << endl;
    cout << "lines, the maximum length, the number of empty lines and the lines"
         << endl;
    cout << "with given first and last characters. Write it to a new file."
         << endl;
}

/*
 * Функція вводить рядки з клавіатури і записує їх у текстовий файл.
 * Рядок може бути порожнім
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void createFile()
{
    int count = 0; // кількість рядків

    outputFile.open(TEXT_FILE);
    if (!outputFile.is_open())
    {
        cout << "Error! Cannot open file " << TEXT_FILE << endl;
    }
    else
    {
        cout << "Enter number of lines (from 1 to " << MAX_LINES << "): ";
        cin >> count;
        while (cin.fail() || count < 1 || count > MAX_LINES)
        {
            cin.clear();            // скинути помилку введення
            cin.ignore(1000, '\n'); // і пропустити неправильний рядок
            cout << "Error! Enter a number from 1 to " << MAX_LINES << ": ";
            cin >> count;
        }
        cin.ignore(1000, '\n'); // прибрати кінець рядка перед cin.getline

        for (int i = 0; i < count; i++)
        {
            cout << "Enter line " << i + 1 << ": ";
            cin.getline(line, LINE_LENGTH);
            while (cin.fail())
            {
                cin.clear();            // скинути помилку введення
                cin.ignore(1000, '\n'); // і пропустити залишок рядка
                cout << "Error! The line is too long. Enter it again: ";
                cin.getline(line, LINE_LENGTH);
            }
            outputFile << line << endl;
        }
        outputFile.close();

        cout << "File " << TEXT_FILE << " is created." << endl;
    }
}

/*
 * Функція виводить вміст файлу на екран
 * Параметри: name - ім'я файлу
 * Значення, що повертаються: немає
 */
void showFile(const char name[])
{
    inputFile.open(name);
    if (!inputFile.is_open())
    {
        cout << "Error! File " << name << " does not exist." << endl;
    }
    else
    {
        cout << endl << "Contents of file " << name << ":" << endl;
        inputFile.getline(line, LINE_LENGTH);
        while (!inputFile.fail())
        {
            cout << line << endl;
            inputFile.getline(line, LINE_LENGTH);
        }
        inputFile.close();
    }
}

/*
 * Функція рахує рядки текстового файлу. Якщо файлу немає, читання
 * не вдається і кількість лишається 0
 * Параметри: немає
 * Значення, що повертаються: кількість рядків
 */
int countLines()
{
    int count = 0;

    inputFile.open(TEXT_FILE);
    inputFile.getline(line, LINE_LENGTH);
    while (!inputFile.fail())
    {
        count++;
        inputFile.getline(line, LINE_LENGTH);
    }
    inputFile.close();

    if (count == 0)
    {
        cout << "Error! File " << TEXT_FILE << " does not exist or is empty."
             << endl;
    }

    return count;
}

/*
 * Функція знаходить максимальну довжину рядка текстового файлу
 * Параметри: немає
 * Значення, що повертаються: максимальна довжина
 */
int findMaxLength()
{
    int maxLength = 0;
    int length = 0;

    inputFile.open(TEXT_FILE);
    inputFile.getline(line, LINE_LENGTH);
    while (!inputFile.fail())
    {
        length = strlen(line);
        if (length > maxLength)
        {
            maxLength = length;
        }
        inputFile.getline(line, LINE_LENGTH);
    }
    inputFile.close();

    return maxLength;
}

/*
 * Функція рахує порожні рядки текстового файлу
 * Параметри: немає
 * Значення, що повертаються: кількість порожніх рядків
 */
int countEmptyLines()
{
    int count = 0;

    inputFile.open(TEXT_FILE);
    inputFile.getline(line, LINE_LENGTH);
    while (!inputFile.fail())
    {
        if (strlen(line) == 0)
        {
            count++;
        }
        inputFile.getline(line, LINE_LENGTH);
    }
    inputFile.close();

    return count;
}

/*
 * Функція перевіряє, чи починається і чи завершується поточний рядок
 * заданими символами
 * Параметри: first, last - задані символи
 * Значення, що повертаються: true, якщо рядок підходить
 */
bool isMatchingLine(char first, char last)
{
    bool isMatching = false;
    int length = strlen(line);

    if (length > 0)
    {
        if (line[0] == first && line[length - 1] == last)
        {
            isMatching = true;
        }
    }

    return isMatching;
}

/*
 * Функція вводить задані символи і записує у новий текстовий файл
 * характеристики файлу та рядки, що починаються і завершуються цими
 * символами
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void processFile()
{
    char first = ' ';   // заданий символ на початку рядка
    char last = ' ';    // заданий символ у кінці рядка
    int matchCount = 0; // кількість рядків із заданими символами
    int lineCount = countLines();

    if (lineCount > 0)
    {
        outputFile.open(RESULT_FILE);
        if (!outputFile.is_open())
        {
            cout << "Error! Cannot open file " << RESULT_FILE << endl;
        }
        else
        {
            cout << "Enter first character of the line: ";
            cin >> first;
            cout << "Enter last character of the line: ";
            cin >> last;
            cin.ignore(1000, '\n'); // пропустити решту рядка

            outputFile << "Number of lines: " << lineCount << endl;
            outputFile << "Maximum line length: " << findMaxLength() << endl;
            outputFile << "Number of empty lines: " << countEmptyLines()
                       << endl;
            outputFile << "Lines that start with '" << first
                       << "' and end with '" << last << "':" << endl;

            inputFile.open(TEXT_FILE);
            inputFile.getline(line, LINE_LENGTH);
            while (!inputFile.fail())
            {
                if (isMatchingLine(first, last))
                {
                    outputFile << line << endl;
                    matchCount++;
                }
                inputFile.getline(line, LINE_LENGTH);
            }
            inputFile.close();

            outputFile << "Number of such lines: " << matchCount << endl;
            outputFile.close();
            cout << "Results are written to file " << RESULT_FILE << endl;
        }
    }
}

/*
 * Функція виводить меню і викликає вибрану дію, поки не вибрано вихід
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void runMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        cout << endl << "Menu" << endl;
        cout << "1 - create text file from keyboard" << endl;
        cout << "2 - show text file" << endl;
        cout << "3 - process text file and write results" << endl;
        cout << "4 - show results file" << endl;
        cout << "0 - exit" << endl;
        cout << "Enter menu item: ";
        cin >> choice;
        while (cin.fail())
        {
            cin.clear();            // скинути помилку введення
            cin.ignore(1000, '\n'); // і пропустити неправильний рядок
            cout << "Error! Enter a number: ";
            cin >> choice;
        }
        cin.ignore(1000, '\n'); // прибрати кінець рядка перед cin.getline

        switch (choice)
        {
            case 1 : createFile(); break;
            case 2 : showFile(TEXT_FILE); break;
            case 3 : processFile(); break;
            case 4 : showFile(RESULT_FILE); break;
            case 0 : cout << "The program is finished." << endl; break;
            default : cout << "Error! There is no such menu item." << endl;
        }
    }
}

int main()
{
    welcome();
    runMenu();
    return 0;
}
