/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task2.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 1
// Запланована ціль: лабораторна робота №9, завдання 2, варіант 6.2.
//                   Знайти в тексті ключові слова за тематикою, скласти
//                   з них акроніми і вибрати найкращий.
/////////////////////////////////////////////////////////////////////

#include <cctype>
#include <cstring>
#include <iostream>
using namespace std;

//=========== константи
const int MAX_LINES = 10;            // найбільша кількість рядків тексту
const int MAX_LENGTH = 256;          // найбільший розмір рядка
const int MAX_TOPIC_WORDS = 64;      // слів з 4 літер у рядку не більше
const int MAX_KEYWORDS = 12;         // найбільша кількість ключових слів
const int MAX_ACRONYMS = 66;         // пар "перше - останнє" ключове слово
const int MATCH_LENGTH = 4;          // скільки перших літер має збігатися
const char SEPARATORS[] = " ,.;:!?"; // розділові символи
const char VOWELS[] = "AEIOUY";      // голосні літери

//=========== глобальні змінні
char text[MAX_LINES][MAX_LENGTH] = {""};              // введений текст
char topic[MAX_LENGTH] = "";                          // тематика тексту
char topicWords[MAX_TOPIC_WORDS][MAX_LENGTH] = {""};  // слова тематики
char keywords[MAX_KEYWORDS][MAX_LENGTH] = {""};       // знайдені ключові слова
char acronyms[MAX_ACRONYMS][MAX_KEYWORDS + 1] = {""}; // згенеровані акроніми
int lineCount = 0;                                    // кількість рядків тексту
int topicWordCount = 0;                               // кількість слів тематики
int keywordCount = 0;                                 // кількість ключових слів
int acronymCount = 0;                                 // кількість акронімів

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 9. Task 2. Variant 6." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;

    cout << "Find keywords of a text by its topic, make acronyms from them"
         << endl;
    cout << "and choose the best acronym." << endl << endl;
}

/*
 * Функція вводить один рядок. Занадто довгий рядок вводиться ще раз
 * Параметри: target - куди записати рядок
 * Значення, що повертаються: немає
 */
void inputLine(char* target)
{
    cin.getline(target, MAX_LENGTH);
    while (cin.fail())
    {
        cin.clear();            // скинути помилку введення
        cin.ignore(1000, '\n'); // і пропустити занадто довгий рядок
        cout << "Error! The line is too long. Enter it again: ";
        cin.getline(target, MAX_LENGTH);
    }
}

/*
 * Функція вводить кількість рядків і сам текст
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputText()
{
    cout << "Enter the number of lines (from 1 to " << MAX_LINES << "): ";
    cin >> lineCount;
    while (cin.fail() || lineCount < 1 || lineCount > MAX_LINES)
    {
        cin.clear();            // скинути помилку введення
        cin.ignore(1000, '\n'); // і пропустити неправильний рядок
        cout << "Error! Enter the number of lines from 1 to " << MAX_LINES
             << ": ";
        cin >> lineCount;
    }
    cin.ignore(1000, '\n'); // кінець рядка після числа

    for (int i = 0; i < lineCount; i++)
    {
        cout << "Line " << i + 1 << ": ";
        inputLine(text[i]);
    }
}

/*
 * Функція перевіряє, що слово починається з літери і не коротше
 * за MATCH_LENGTH символів
 * Параметри: word - слово, яке перевіряється
 * Значення, що повертаються: true, якщо слово можна брати до уваги
 */
bool isLongWord(char* word)
{
    bool isLong = false;
    int length = strlen(word);

    if (isalpha(word[0]) && length >= MATCH_LENGTH)
    {
        isLong = true;
    }

    return isLong;
}

/*
 * Функція переводить слово в нижній регістр, щоб порівняння не
 * залежало від великих літер
 * Параметри: word - слово
 * Значення, що повертаються: немає
 */
void toLowerCase(char* word)
{
    for (int i = 0; word[i] != '\0'; i++)
    {
        word[i] = tolower(word[i]);
    }
}

/*
 * Функція вводить тематику і виділяє з неї слова з 4 і більше літер.
 * Якщо таких слів немає, тематика вводиться ще раз
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputTopic()
{
    char copy[MAX_LENGTH];
    char* word = NULL;

    do
    {
        cout << "Enter the topic of the text: ";
        inputLine(topic);
        strcpy(copy, topic);
        topicWordCount = 0;

        word = strtok(copy, SEPARATORS);
        while (word != NULL)
        {
            toLowerCase(word);
            if (isLongWord(word))
            {
                strcpy(topicWords[topicWordCount], word);
                topicWordCount++;
            }
            word = strtok(NULL, SEPARATORS);
        }

        if (topicWordCount == 0)
        {
            cout << "Error! The topic needs a word of at least " << MATCH_LENGTH
                 << " letters." << endl;
        }
    } while (topicWordCount == 0);
}

/*
 * Функція перевіряє, чи відповідає слово тематиці: його початок
 * має збігатися з початком хоча б одного слова тематики
 * Параметри: word - слово в нижньому регістрі
 * Значення, що повертаються: true, якщо слово відповідає тематиці
 */
bool isTopicWord(char* word)
{
    bool isFound = false;

    for (int i = 0; i < topicWordCount; i++)
    {
        if (strncmp(word, topicWords[i], MATCH_LENGTH) == 0)
        {
            isFound = true;
        }
    }

    return isFound;
}

/*
 * Функція додає слово до ключових, якщо його там ще немає
 * і в масиві є місце
 * Параметри: word - слово в нижньому регістрі
 * Значення, що повертаються: немає
 */
void addKeyword(char* word)
{
    bool isFound = false;

    for (int i = 0; i < keywordCount; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
        {
            isFound = true;
        }
    }

    if (!isFound && keywordCount < MAX_KEYWORDS)
    {
        strcpy(keywords[keywordCount], word);
        keywordCount++;
    }
}

/*
 * Функція переглядає текст і збирає ключові слова
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void findKeywords()
{
    char copy[MAX_LENGTH];
    char* word = NULL;

    for (int i = 0; i < lineCount; i++)
    {
        strcpy(copy, text[i]);
        word = strtok(copy, SEPARATORS);
        while (word != NULL)
        {
            toLowerCase(word);
            if (isLongWord(word) && isTopicWord(word))
            {
                addKeyword(word);
            }
            word = strtok(NULL, SEPARATORS);
        }
    }

    cout << endl << "Keywords: " << keywordCount << endl;
    for (int i = 0; i < keywordCount; i++)
    {
        cout << i + 1 << ". " << keywords[i] << endl;
    }
}

/*
 * Функція додає акронім, якщо такого ще немає
 * Параметри: acronym - новий акронім
 * Значення, що повертаються: немає
 */
void addAcronym(char* acronym)
{
    bool isFound = false;

    for (int i = 0; i < acronymCount; i++)
    {
        if (strcmp(acronym, acronyms[i]) == 0)
        {
            isFound = true;
        }
    }

    if (!isFound)
    {
        strcpy(acronyms[acronymCount], acronym);
        acronymCount++;
    }
}

/*
 * Функція складає акроніми з перших літер кожного набору ключових слів,
 * що йдуть поспіль і містять щонайменше два слова
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void generateAcronyms()
{
    char acronym[MAX_KEYWORDS + 1];

    for (int first = 0; first < keywordCount; first++)
    {
        acronym[0] = toupper(keywords[first][0]);
        for (int last = first + 1; last < keywordCount; last++)
        {
            acronym[last - first] = toupper(keywords[last][0]);
            acronym[last - first + 1] = '\0';
            addAcronym(acronym);
        }
    }
}

/*
 * Функція рахує оцінку акроніма: кількість пар сусідніх літер, де
 * голосна стоїть поруч із приголосною. Чим більше таких пар, тим легше
 * вимовити акронім
 * Параметри: acronym - акронім
 * Значення, що повертаються: оцінка
 */
int calculateScore(char* acronym)
{
    int score = 0;

    for (int i = 0; acronym[i + 1] != '\0'; i++)
    {
        if (strchr(VOWELS, acronym[i]) != NULL
            && strchr(VOWELS, acronym[i + 1]) == NULL)
        {
            score++;
        }
        else if (strchr(VOWELS, acronym[i]) == NULL
                 && strchr(VOWELS, acronym[i + 1]) != NULL)
        {
            score++;
        }
    }

    return score;
}

/*
 * Функція виводить усі акроніми з оцінками і найкращий акронім. За
 * однакової оцінки найкращим лишається той, що знайдено раніше
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void printAcronyms()
{
    int bestScore = -1;
    int bestNumber = 0;

    cout << endl << "Acronyms: " << acronymCount << endl;
    for (int i = 0; i < acronymCount; i++)
    {
        cout << i + 1 << ". " << acronyms[i] << ", score "
             << calculateScore(acronyms[i]) << endl;
        if (calculateScore(acronyms[i]) > bestScore)
        {
            bestScore = calculateScore(acronyms[i]);
            bestNumber = i;
        }
    }

    if (acronymCount == 0)
    {
        cout << "At least two keywords are needed." << endl;
    }
    else
    {
        cout << "Best acronym: " << acronyms[bestNumber] << endl;
    }
}

int main()
{
    welcome();
    inputText();
    inputTopic();
    findKeywords();
    generateAcronyms();
    printAcronyms();
    return 0;
}
