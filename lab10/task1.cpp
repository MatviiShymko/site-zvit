/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task1.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 2
// Запланована ціль: лабораторна робота №10, варіант 5.
//                   Створити масив структур про факультети інститутів
//                   і виконати запити за спеціальностями.
/////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
using namespace std;

//=========== константи
const int MAX_RECORDS = 10;    // найбільша кількість записів масиву
const int MAX_SPECIALTIES = 3; // найбільша кількість спеціальностей у записі
const int CITY_LENGTH = 15;    // довжини рядків разом із символом кінця рядка,
const int INSTITUTE_LENGTH = 22; // вони ж ширини колонок таблиці
const int FACULTY_LENGTH = 22;
const int SPECIALTY_LENGTH = 27;
const int NUMBER_WIDTH = 4; // ширина колонки з номером
const int MAX_PLAN = 500;   // найбільший план прийому на спеціальність
const int CITY_TOTAL = 3;   // кількість назв для генерації
const int INSTITUTE_TOTAL = 2;
const int FACULTY_TOTAL = 2;
const int SPECIALTY_TOTAL = 4;

// назви, з яких генератор випадкових чисел складає записи
const char CITIES[CITY_TOTAL][CITY_LENGTH] = {"Kyiv", "Lviv", "Odesa"};
const char INSTITUTES[INSTITUTE_TOTAL][INSTITUTE_LENGTH] = {
    "Polytechnic Institute", "State University"};
const char FACULTIES[FACULTY_TOTAL][FACULTY_LENGTH] = {"Informatics",
                                                       "Cybernetics"};
const char SPECIALTIES[SPECIALTY_TOTAL][SPECIALTY_LENGTH] = {
    "Computer Science", "Software Engineering", "Computer Engineering",
    "Automated Control Systems"};

// заголовок таблиці, колонки відповідають ширинам вище
const char TABLE_HEADER[] = "No  City           Institute             "
                            "Faculty               Specialty                  "
                            "Plan";

//=========== структури
struct Specialty // спеціальність зі своїм планом прийому
{
    char name[SPECIALTY_LENGTH]; // назва спеціальності
    int plan;                    // план прийому на перший курс
};

struct Faculty // один запис масиву
{
    char city[CITY_LENGTH];                 // місто
    char institute[INSTITUTE_LENGTH];       // інститут
    char name[FACULTY_LENGTH];              // факультет
    Specialty specialties[MAX_SPECIALTIES]; // список спеціальностей
    int specialtyCount;                     // кількість спеціальностей
};

//=========== глобальні змінні
Faculty records[MAX_RECORDS] = {}; // масив структур
int recordCount = 0;               // кількість записів у масиві

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 10. Variant 5." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;

    cout
        << "Array of structures: city, institute, faculty, list of specialties,"
        << endl;
    cout << "admission plan to the first year (Plan in the tables)." << endl;
}

/*
 * Функція вводить ціле число із заданих меж. Якщо введено не число
 * або число поза межами, введення повторюється
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
    cin.ignore(1000, '\n'); // прибрати кінець рядка перед cin.getline

    return number;
}

/*
 * Функція вводить непорожній рядок. Якщо рядок порожній або задовгий,
 * введення повторюється
 * Параметри: text - куди записати рядок; maxLength - розмір масиву text
 * Значення, що повертаються: немає
 */
void inputText(char* text, int maxLength)
{
    cin.getline(text, maxLength);
    while (cin.fail() || strlen(text) == 0)
    {
        if (cin.fail())
        {
            cin.clear();            // скинути помилку введення
            cin.ignore(1000, '\n'); // і пропустити залишок рядка
        }
        cout << "Error! Enter text, maximum length is " << maxLength - 1
             << ": ";
        cin.getline(text, maxLength);
    }
}

/*
 * Функція вводить з клавіатури один запис
 * Параметри: немає
 * Значення, що повертаються: заповнена структура
 */
Faculty inputRecord()
{
    Faculty record = {};

    cout << "Enter city: ";
    inputText(record.city, CITY_LENGTH);
    cout << "Enter institute: ";
    inputText(record.institute, INSTITUTE_LENGTH);
    cout << "Enter faculty: ";
    inputText(record.name, FACULTY_LENGTH);

    cout << "Enter number of specialties (from 1 to " << MAX_SPECIALTIES
         << "): ";
    record.specialtyCount = inputNumber(1, MAX_SPECIALTIES);
    for (int i = 0; i < record.specialtyCount; i++)
    {
        cout << "Enter specialty " << i + 1 << ": ";
        inputText(record.specialties[i].name, SPECIALTY_LENGTH);
        cout << "Enter plan (from 1 to " << MAX_PLAN << "): ";
        record.specialties[i].plan = inputNumber(1, MAX_PLAN);
    }

    return record;
}

/*
 * Функція складає один запис із випадкових назв і чисел. Спеціальності
 * беруться зі списку поспіль, тому в записі вони не повторюються
 * Параметри: немає
 * Значення, що повертаються: заповнена структура
 */
Faculty generateRecord()
{
    Faculty record = {};
    int firstIndex = rand() % SPECIALTY_TOTAL; // перша спеціальність запису

    strcpy(record.city, CITIES[rand() % CITY_TOTAL]);
    strcpy(record.institute, INSTITUTES[rand() % INSTITUTE_TOTAL]);
    strcpy(record.name, FACULTIES[rand() % FACULTY_TOTAL]);

    record.specialtyCount = MAX_SPECIALTIES;
    for (int i = 0; i < record.specialtyCount; i++)
    {
        strcpy(record.specialties[i].name,
               SPECIALTIES[(firstIndex + i) % SPECIALTY_TOTAL]);
        record.specialties[i].plan = 1 + rand() % MAX_PLAN;
    }

    return record;
}

/*
 * Функція виводить рядок таблиці: запис з однією його спеціальністю
 * Параметри: record - покажчик на запис; index - номер спеціальності;
 *            number - номер у таблиці
 * Значення, що повертаються: немає
 */
void printRow(Faculty* record, int index, int number)
{
    cout << left << setw(NUMBER_WIDTH) << number << setw(CITY_LENGTH)
         << record->city << setw(INSTITUTE_LENGTH) << record->institute
         << setw(FACULTY_LENGTH) << record->name << setw(SPECIALTY_LENGTH)
         << record->specialties[index].name << record->specialties[index].plan
         << endl;
}

/*
 * Функція виводить масив структур таблицею
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void printArray()
{
    cout << endl << TABLE_HEADER << endl;
    for (int i = 0; i < recordCount; i++)
    {
        for (int j = 0; j < records[i].specialtyCount; j++)
        {
            printRow(&records[i], j, i + 1);
        }
    }
}

/*
 * Функція створює масив і виводить його таблицею
 * Параметри: isRandom - true для генерації, false для введення з клавіатури
 * Значення, що повертаються: немає
 */
void createArray(bool isRandom)
{
    cout << "Enter number of records (from 1 to " << MAX_RECORDS << "): ";
    recordCount = inputNumber(1, MAX_RECORDS);

    for (int i = 0; i < recordCount; i++)
    {
        if (isRandom)
        {
            records[i] = generateRecord();
        }
        else
        {
            cout << "Record " << i + 1 << endl;
            records[i] = inputRecord();
        }
    }

    printArray();
}

/*
 * Функція виводить спеціальності, у назвах яких є хоча б одне
 * з двох слів, заданих з клавіатури
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void findByWords()
{
    char firstWord[SPECIALTY_LENGTH] = "";
    char secondWord[SPECIALTY_LENGTH] = "";
    int count = 0; // кількість знайдених рядків

    cout << "Enter first word: ";
    inputText(firstWord, SPECIALTY_LENGTH);
    cout << "Enter second word: ";
    inputText(secondWord, SPECIALTY_LENGTH);

    cout << endl << TABLE_HEADER << endl;
    for (int i = 0; i < recordCount; i++)
    {
        for (int j = 0; j < records[i].specialtyCount; j++)
        {
            if (strstr(records[i].specialties[j].name, firstWord) != NULL
                || strstr(records[i].specialties[j].name, secondWord) != NULL)
            {
                count++;
                printRow(&records[i], j, count);
            }
        }
    }

    if (count == 0)
    {
        cout << "Nothing found." << endl;
    }
}

/*
 * Функція виводить факультети й інститути, де є задана спеціальність
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void findBySpecialty()
{
    char specialty[SPECIALTY_LENGTH] = "";
    int count = 0; // кількість знайдених рядків

    cout << "Enter specialty: ";
    inputText(specialty, SPECIALTY_LENGTH);

    cout << endl << TABLE_HEADER << endl;
    for (int i = 0; i < recordCount; i++)
    {
        for (int j = 0; j < records[i].specialtyCount; j++)
        {
            if (strcmp(records[i].specialties[j].name, specialty) == 0)
            {
                count++;
                printRow(&records[i], j, count);
            }
        }
    }

    if (count == 0)
    {
        cout << "Nothing found." << endl;
    }
}

/*
 * Функція знаходить найбільший план на задану спеціальність і виводить
 * факультет, інститут і місто. Якщо такий план є в кількох записах,
 * виводяться всі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void findMaxPlan()
{
    char specialty[SPECIALTY_LENGTH] = "";
    int maxPlan = 0; // найбільший план на спеціальність
    int count = 0;   // кількість знайдених рядків

    cout << "Enter specialty: ";
    inputText(specialty, SPECIALTY_LENGTH);

    for (int i = 0; i < recordCount; i++)
    {
        for (int j = 0; j < records[i].specialtyCount; j++)
        {
            if (strcmp(records[i].specialties[j].name, specialty) == 0
                && records[i].specialties[j].plan > maxPlan)
            {
                maxPlan = records[i].specialties[j].plan;
            }
        }
    }

    cout << endl << TABLE_HEADER << endl;
    for (int i = 0; i < recordCount; i++)
    {
        for (int j = 0; j < records[i].specialtyCount; j++)
        {
            if (strcmp(records[i].specialties[j].name, specialty) == 0
                && records[i].specialties[j].plan == maxPlan)
            {
                count++;
                printRow(&records[i], j, count);
            }
        }
    }

    if (count == 0)
    {
        cout << "Nothing found." << endl;
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

    srand(time(NULL));
    while (choice != 0)
    {
        cout << endl << "Menu" << endl;
        cout << "1 - create array from keyboard" << endl;
        cout << "2 - generate random array" << endl;
        cout << "3 - print array" << endl;
        cout << "4 - find specialties by two words" << endl;
        cout << "5 - find faculties and institutes with a specialty" << endl;
        cout << "6 - find the largest plan for a specialty" << endl;
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

        if (recordCount == 0 && choice >= 3 && choice <= 6)
        {
            cout << "Error! The array is empty. Create it first." << endl;
        }
        else
        {
            switch (choice)
            {
                case 1 : createArray(false); break;
                case 2 : createArray(true); break;
                case 3 : printArray(); break;
                case 4 : findByWords(); break;
                case 5 : findBySpecialty(); break;
                case 6 : findMaxPlan(); break;
                case 0 : cout << "The program is finished." << endl; break;
                default : cout << "Error! There is no such menu item." << endl;
            }
        }
    }
}

int main()
{
    welcome();
    runMenu();
    return 0;
}
