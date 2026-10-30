/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task3.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 1
// Запланована ціль: лабораторна робота №6, завдання 3, варіант 6.
//                   Поділити многочлен степеня m на многочлен степеня n
//                   і перевірити результат.
/////////////////////////////////////////////////////////////////////

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

//=========== константи
const int MIN_DEGREE = 3;         // найменший степінь дільника
const int MAX_DEGREE = 8;         // найбільший степінь діленого
const int MIN_COEFFICIENT = -100; // межі коефіцієнтів
const int MAX_COEFFICIENT = 100;
const double TOLERANCE = 0.000001; // менші за модулем числа вважаємо нулем

//=========== глобальні змінні
// double, бо коефіцієнти частки швидко ростуть, і 7 цифр float не вистачає:
// перевірка показала б різницю, якої насправді немає
double dividend[MAX_DEGREE + 1] = {0};      // коефіцієнти діленого
double divisor[MAX_DEGREE + 1] = {0};       // коефіцієнти дільника
double workArray[MAX_DEGREE + 1] = {0};     // залишок під час ділення
double quotient[MAX_DEGREE + 1] = {0};      // коефіцієнти частки
double remainderPart[MAX_DEGREE + 1] = {0}; // коефіцієнти остачі
double checkResult[MAX_DEGREE + 1] = {0};   // частка * дільник + остача
int dividendDegree = 0;                     // степінь діленого m
int divisorDegree = 0;                      // степінь дільника n
int quotientDegree = 0;                     // степінь частки m - n
int lowerBound = 0;                         // межі випадкових коефіцієнтів
int upperBound = 0;
int choice = 0;        // вибраний пункт меню
bool isCorrect = true; // чи збігся результат перевірки

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 6. Task 3. Variant 6." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;

    cout << "Divide the polynomial Pm(x) by the polynomial Pn(x), m > n > 2:"
         << endl;
    cout << "Pm(x) = b0*x^m + b1*x^(m-1) + ... + bm" << endl;
    cout << "Pn(x) = a0*x^n + a1*x^(n-1) + ... + an" << endl;
    cout << "Then check: Q(x) * Pn(x) + R(x) = Pm(x)." << endl << endl;
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
 * Функція вводить степені многочленів m і n, m > n > 2
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputDegrees()
{
    cout << "Enter degree m (from " << MIN_DEGREE + 1 << " to " << MAX_DEGREE
         << "): ";
    dividendDegree = inputNumber(MIN_DEGREE + 1, MAX_DEGREE);

    cout << "Enter degree n (from " << MIN_DEGREE << " to "
         << dividendDegree - 1 << "): ";
    divisorDegree = inputNumber(MIN_DEGREE, dividendDegree - 1);

    quotientDegree = dividendDegree - divisorDegree;
}

/*
 * Функція вводить коефіцієнти обох многочленів з клавіатури.
 * Старший коефіцієнт дільника не може дорівнювати нулю
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void inputCoefficients()
{
    cout << endl
         << "Coefficients of Pm(x) (integers from " << MIN_COEFFICIENT << " to "
         << MAX_COEFFICIENT << "):" << endl;
    for (int i = 0; i <= dividendDegree; i++)
    {
        cout << "b" << i << " (at x^" << dividendDegree - i << "): ";
        dividend[i] = inputNumber(MIN_COEFFICIENT, MAX_COEFFICIENT);
    }

    cout << "Coefficients of Pn(x) (integers from " << MIN_COEFFICIENT << " to "
         << MAX_COEFFICIENT << "):" << endl;
    cout << "a0 (at x^" << divisorDegree << "): ";
    divisor[0] = inputNumber(MIN_COEFFICIENT, MAX_COEFFICIENT);
    while (divisor[0] == 0)
    {
        cout << "Error! a0 must not be 0. Enter a0: ";
        divisor[0] = inputNumber(MIN_COEFFICIENT, MAX_COEFFICIENT);
    }

    for (int i = 1; i <= divisorDegree; i++)
    {
        cout << "a" << i << " (at x^" << divisorDegree - i << "): ";
        divisor[i] = inputNumber(MIN_COEFFICIENT, MAX_COEFFICIENT);
    }
}

/*
 * Функція вводить діапазон і заповнює коефіцієнти псевдовипадковими
 * цілими числами. Нижня межа менша за верхню, тому старший коефіцієнт
 * дільника рано чи пізно вийде відмінним від нуля
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void generateCoefficients()
{
    cout << endl
         << "Enter the lower bound (from " << MIN_COEFFICIENT << " to "
         << MAX_COEFFICIENT - 1 << "): ";
    lowerBound = inputNumber(MIN_COEFFICIENT, MAX_COEFFICIENT - 1);

    cout << "Enter the upper bound (from " << lowerBound + 1 << " to "
         << MAX_COEFFICIENT << "): ";
    upperBound = inputNumber(lowerBound + 1, MAX_COEFFICIENT);

    srand(time(NULL));
    for (int i = 0; i <= dividendDegree; i++)
    {
        dividend[i] = rand() % (upperBound - lowerBound + 1) + lowerBound;
    }

    for (int i = 0; i <= divisorDegree; i++)
    {
        divisor[i] = rand() % (upperBound - lowerBound + 1) + lowerBound;
    }

    while (divisor[0] == 0)
    {
        divisor[0] = rand() % (upperBound - lowerBound + 1) + lowerBound;
    }
}

/*
 * Функція виводить меню і створює коефіцієнти вибраним способом
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void runMenu()
{
    cout << endl << "How to create the coefficients:" << endl;
    cout << "1 - enter from the keyboard" << endl;
    cout << "2 - generate pseudorandom numbers in a given range" << endl;
    cout << "Your choice: ";
    choice = inputNumber(1, 2);

    switch (choice)
    {
        case 1 : inputCoefficients(); break;
        case 2 : generateCoefficients(); break;
        default : cout << "There is no such menu item." << endl; break;
    }
}

/*
 * Функція виводить многочлен у вигляді виразу, нульові члени пропускає
 * Параметри: coefficients - коефіцієнти від старшого до вільного члена,
 *            degree - степінь многочлена
 * Значення, що повертаються: немає
 */
void printPolynomial(double coefficients[], int degree)
{
    bool isFirst = true; // чи ще не виведено жодного члена
    int power = 0;       // степінь x у поточному члені

    for (int i = 0; i <= degree; i++)
    {
        power = degree - i;
        if (fabs(coefficients[i]) > TOLERANCE)
        {
            if (coefficients[i] < 0 && isFirst)
            {
                cout << "-";
            }
            else if (coefficients[i] < 0)
            {
                cout << " - ";
            }
            else if (!isFirst)
            {
                cout << " + ";
            }

            cout << fabs(coefficients[i]);
            if (power > 0)
            {
                cout << "x";
            }
            if (power > 1)
            {
                cout << "^" << power;
            }
            isFirst = false;
        }
    }

    if (isFirst)
    {
        cout << "0";
    }
    cout << endl;
}

/*
 * Функція ділить многочлени стовпчиком. Кожен коефіцієнт частки це
 * старший коефіцієнт залишку, поділений на старший коефіцієнт дільника.
 * Від залишку віднімається дільник, помножений на цей коефіцієнт
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void dividePolynomials()
{
    for (int i = 0; i <= dividendDegree; i++)
    {
        workArray[i] = dividend[i];
    }

    for (int i = 0; i <= quotientDegree; i++)
    {
        quotient[i] = workArray[i] / divisor[0];
        for (int j = 0; j <= divisorDegree; j++)
        {
            workArray[i + j] = workArray[i + j] - quotient[i] * divisor[j];
        }
    }

    for (int i = 0; i < divisorDegree; i++)
    {
        remainderPart[i] = workArray[quotientDegree + 1 + i];
    }
}

/*
 * Функція множить частку на дільник, додає остачу і порівнює
 * результат із діленим
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void checkDivision()
{
    for (int i = 0; i <= dividendDegree; i++)
    {
        checkResult[i] = 0;
    }

    for (int i = 0; i <= quotientDegree; i++)
    {
        for (int j = 0; j <= divisorDegree; j++)
        {
            checkResult[i + j] = checkResult[i + j] + quotient[i] * divisor[j];
        }
    }

    for (int i = 0; i < divisorDegree; i++)
    {
        checkResult[quotientDegree + 1 + i] =
            checkResult[quotientDegree + 1 + i] + remainderPart[i];
    }

    isCorrect = true;
    for (int i = 0; i <= dividendDegree; i++)
    {
        if (fabs(checkResult[i] - dividend[i]) > TOLERANCE)
        {
            isCorrect = false;
        }
    }
}

/*
 * Функція виводить многочлени, частку, остачу і результат перевірки
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void printResults()
{
    cout << endl << "Pm(x) = ";
    printPolynomial(dividend, dividendDegree);
    cout << "Pn(x) = ";
    printPolynomial(divisor, divisorDegree);

    cout << endl << "Quotient  Q(x) = ";
    printPolynomial(quotient, quotientDegree);
    cout << "Remainder R(x) = ";
    printPolynomial(remainderPart, divisorDegree - 1);

    cout << endl << "Check: Q(x) * Pn(x) + R(x) = ";
    printPolynomial(checkResult, dividendDegree);

    if (isCorrect)
    {
        cout << "The result of the check is equal to Pm(x)." << endl;
    }
    else
    {
        cout << "The result of the check is NOT equal to Pm(x)." << endl;
    }
}

int main()
{
    welcome();
    inputDegrees();
    runMenu();
    dividePolynomials();
    checkDivision();
    printResults();
    return 0;
}
