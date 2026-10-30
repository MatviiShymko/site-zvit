/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task1.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 1
// Запланована ціль: лабораторна робота №9, завдання 1, варіант 6.1.
//                   Виділити слова в рядку, розбити кожне друге слово на
//                   склади, видалити односкладові слова і замінити
//                   двоскладові на "...".
/////////////////////////////////////////////////////////////////////

#include <cstring>
#include <iostream>
using namespace std;

//=========== константи
const int MAX_LENGTH = 256;           // найбільший розмір рядка
const int MAX_WORDS = MAX_LENGTH / 2; // більше слів у рядок не вміститься
const char SEPARATORS[] = " ,.";      // розділові символи
const char VOWELS[] = "aeiouyAEIOUY"; // голосні літери

//=========== глобальні змінні
char line[MAX_LENGTH] = "";               // введений рядок
char words[MAX_WORDS][MAX_LENGTH] = {""}; // масив слів
int wordCount = 0;                        // кількість слів
int choice = 0;                           // номер команди меню

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 9. Task 1. Variant 6." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;

    cout << "Find the words of a line, split every second word into syllables,"
         << endl;
    cout << "remove one-syllable words and replace two-syllable words with ..."
         << endl
         << endl;
}

/*
 * Функція вводить рядок. Занадто довгий рядок або рядок без слів
 * вводиться ще раз
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputLine()
{
    cout << "Enter a line: ";
    cin.getline(line, MAX_LENGTH);
    while (cin.fail() || strspn(line, SEPARATORS) == strlen(line))
    {
        if (cin.fail())
        {
            cin.clear();            // скинути помилку введення
            cin.ignore(1000, '\n'); // і пропустити занадто довгий рядок
        }
        cout << "Error! Enter up to " << MAX_LENGTH - 1
             << " characters with at least one word: ";
        cin.getline(line, MAX_LENGTH);
    }
}

/*
 * Функція розбиває рядок на слова. Функція strtok псує рядок, тому
 * працює з його копією
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void extractWords()
{
    char copy[MAX_LENGTH];
    char* word = NULL;

    strcpy(copy, line);
    wordCount = 0;

    word = strtok(copy, SEPARATORS);
    while (word != NULL)
    {
        strcpy(words[wordCount], word);
        wordCount++;
        word = strtok(NULL, SEPARATORS);
    }
}

/*
 * Функція перевіряє, чи є символ голосною літерою
 * Параметри: symbol - символ, який перевіряється
 * Значення, що повертаються: true, якщо це голосна літера
 */
bool isVowel(char symbol)
{
    bool isFound = false;

    if (symbol != '\0')
    {
        if (strchr(VOWELS, symbol) != NULL)
        {
            isFound = true;
        }
    }

    return isFound;
}

/*
 * Функція розбиває слово на склади. Кілька голосних поспіль дають один
 * склад, склад закінчується голосною, а приголосні між двома голосними
 * діляться навпіл
 * Параметри: word - слово, parts - слово з дефісами між складами
 * Значення, що повертаються: кількість складів
 */
int splitIntoSyllables(char* word, char* parts)
{
    int syllableCount = 0;
    int consonantCount = 0; // приголосні до наступної голосної
    int i = 0;

    parts[0] = '\0';
    while (word[i] != '\0')
    {
        strncat(parts, word + i, 1);
        if (isVowel(word[i]) && !isVowel(word[i + 1]))
        {
            syllableCount++;
            consonantCount = strcspn(word + i + 1, VOWELS);

            if (word[i + 1 + consonantCount] != '\0')
            {
                strncat(parts, word + i + 1, consonantCount / 2);
                strcat(parts, "-");
                i = i + consonantCount / 2;
            }
        }
        i++;
    }

    // слово без голосних вважається односкладовим
    if (syllableCount == 0)
    {
        syllableCount = 1;
    }

    return syllableCount;
}

/*
 * Команда меню: виділяє слова і виводить масив слів та їх кількість
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void printWords()
{
    extractWords();

    for (int i = 0; i < wordCount; i++)
    {
        cout << i + 1 << ". " << words[i] << endl;
    }
    cout << "Number of words: " << wordCount << endl;
}

/*
 * Команда меню: виводить кожне друге слово, розбите на склади
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void printSyllables()
{
    char parts[2 * MAX_LENGTH]; // слово з дефісами

    extractWords();

    if (wordCount < 2)
    {
        cout << "The line has no second word." << endl;
    }

    for (int i = 1; i < wordCount; i = i + 2)
    {
        splitIntoSyllables(words[i], parts);
        cout << i + 1 << ". " << parts << endl;
    }
}

/*
 * Команда меню: будує новий рядок. Розділові символи копіюються,
 * односкладове слово видаляється разом із символами після нього,
 * двоскладове замінюється на "...", довші слова лишаються
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void transformLine()
{
    char word[MAX_LENGTH];
    char parts[2 * MAX_LENGTH];
    char result[2 * MAX_LENGTH] = ""; // новий рядок
    int position = 0;                 // поточне місце в рядку line
    int length = 0;                   // довжина поточного слова
    int syllableCount = 0;

    while (line[position] != '\0')
    {
        length = strcspn(line + position, SEPARATORS);

        if (length == 0)
        {
            strncat(result, line + position, 1);
            position++;
        }
        else
        {
            strncpy(word, line + position, length);
            word[length] = '\0'; // strncpy не ставить кінець рядка
            position = position + length;
            syllableCount = splitIntoSyllables(word, parts);

            if (syllableCount == 1)
            {
                position = position + strspn(line + position, SEPARATORS);
            }
            else if (syllableCount == 2)
            {
                strcat(result, "...");
            }
            else
            {
                strcat(result, word);
            }
        }
    }

    cout << "Source line: " << line << endl;
    if (result[0] == '\0')
    {
        cout << "All words are removed, the new line is empty." << endl;
    }
    else
    {
        cout << "New line:    " << result << endl;
    }
}

/*
 * Функція показує меню і виконує вибрані команди
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void runMenu()
{
    do
    {
        cout << endl << "1 - find the words and count them" << endl;
        cout << "2 - split every second word into syllables" << endl;
        cout << "3 - remove one-syllable words, replace two-syllable words"
             << endl;
        cout << "0 - exit" << endl;
        cout << "Enter a command (from 0 to 3): ";
        cin >> choice;
        while (cin.fail() || choice < 0 || choice > 3)
        {
            cin.clear();            // скинути помилку введення
            cin.ignore(1000, '\n'); // і пропустити неправильний рядок
            cout << "Error! Enter a command from 0 to 3: ";
            cin >> choice;
        }

        switch (choice)
        {
            case 1 : printWords(); break;
            case 2 : printSyllables(); break;
            case 3 : transformLine(); break;
            default : cout << "End of work." << endl; break;
        }
    } while (choice != 0);
}

int main()
{
    welcome();
    inputLine();
    runMenu();
    return 0;
}
