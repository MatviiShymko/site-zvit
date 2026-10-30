/////////////////////////////////////////////////////////////////////
// Ім'я файлу: task2.cpp
// Автор: Шимко Матвій, гр. ІПЗ-11(3)
// Номер версії модуля: version 2
// Запланована ціль: лабораторна робота №2, завдання 2, варіант 10.
//                   Перевірити, чи належить точка площини трикутнику
//                   і багатокутнику, вершини яких вводяться.
/////////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>
using namespace std;

//=========== константи
const int MIN_VERTICES = 3;      // найменша кількість вершин багатокутника
const float ACCURACY = 0.00001f; // відносний допуск порівняння чисел float

//=========== глобальні змінні
float pointX = 0;       // абсциса точки, що перевіряється
float pointY = 0;       // ордината точки, що перевіряється
float vertexAX = 0;     // абсциса вершини A трикутника
float vertexAY = 0;     // ордината вершини A
float vertexBX = 0;     // абсциса вершини B
float vertexBY = 0;     // ордината вершини B
float vertexCX = 0;     // абсциса вершини C
float vertexCY = 0;     // ордината вершини C
float triangleArea = 0; // площа трикутника ABC
float areasSum = 0;     // сума площ PAB, PBC, PCA
int vertexCount = 0;    // кількість вершин багатокутника
float startX = 0;       // абсциса першої вершини багатокутника
float startY = 0;       // ордината першої вершини
float previousX = 0;    // абсциса попередньої вершини
float previousY = 0;    // ордината попередньої вершини
float currentX = 0;     // абсциса поточної вершини
float currentY = 0;     // ордината поточної вершини
int crossingCount = 0;  // кількість перетинів променя зі сторонами
bool isOnSide = false;  // чи лежить точка на стороні багатокутника
float crossingX = 0;    // абсциса перетину сторони з променем
int i = 0;              // лічильник вершин

/*
 * Функція виводить дані про автора та умову задачі
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void welcome()
{
    cout << "Lab work 2. Task 2. Variant 10." << endl;
    cout << "Author: Matvii Shimko, group IPZ-11(3)" << endl << endl;
    cout << "Check whether the point (x, y) belongs to" << endl;
    cout << "- the triangle A(x1, y1), B(x2, y2), C(x3, y3)" << endl;
    cout << "- the polygon A1(x1, y1), ..., An(xn, yn)" << endl << endl;
}

/*
 * Функція вводить одну координату. Якщо введено не число,
 * введення повторюється. Підказку виводить той, хто її викликає
 * Параметри: немає
 * Значення, що повертаються: введена координата
 */
float inputCoordinate()
{
    float coordinate = 0;

    cin >> coordinate;
    while (cin.fail())
    {
        cin.clear();            // скинути помилку введення
        cin.ignore(1000, '\n'); // і пропустити неправильний рядок
        cout << "Error! Enter a number: ";
        cin >> coordinate;
    }

    return coordinate;
}

/*
 * Функція обчислює площу трикутника за координатами його вершин
 * Параметри: firstX, firstY, secondX, secondY, thirdX, thirdY - вершини
 * Значення, що повертаються: площа трикутника
 */
float calculateArea(float firstX,
                    float firstY,
                    float secondX,
                    float secondY,
                    float thirdX,
                    float thirdY)
{
    float doubledArea = (secondX - firstX) * (thirdY - firstY)
                        - (thirdX - firstX) * (secondY - firstY);

    return fabs(doubledArea) / 2;
}

/*
 * Функція перевіряє, чи лежать три точки на одній прямій. Для цього
 * порівнює два добутки різниць координат: на одній прямій вони рівні.
 * Допуск залежить від самих чисел, бо float має 6-7 значущих цифр
 * Параметри: firstX, firstY, secondX, secondY, thirdX, thirdY - точки
 * Значення, що повертаються: true, якщо точки лежать на одній прямій
 */
bool isOnOneLine(float firstX,
                 float firstY,
                 float secondX,
                 float secondY,
                 float thirdX,
                 float thirdY)
{
    bool isOnLine = false;
    float firstProduct = (secondX - firstX) * (thirdY - firstY);
    float secondProduct = (thirdX - firstX) * (secondY - firstY);

    if (fabs(firstProduct - secondProduct)
        <= ACCURACY * (fabs(firstProduct) + fabs(secondProduct)))
    {
        isOnLine = true;
    }

    return isOnLine;
}

/*
 * Функція вводить вершини трикутника і перевіряє, чи належить йому
 * точка: сума площ PAB, PBC, PCA має дорівнювати площі ABC
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void checkTriangle()
{
    cout << "Triangle" << endl;
    cout << "Enter x and y of vertex A: ";
    vertexAX = inputCoordinate();
    vertexAY = inputCoordinate();
    cout << "Enter x and y of vertex B: ";
    vertexBX = inputCoordinate();
    vertexBY = inputCoordinate();
    cout << "Enter x and y of vertex C: ";
    vertexCX = inputCoordinate();
    vertexCY = inputCoordinate();

    if (isOnOneLine(vertexAX, vertexAY, vertexBX, vertexBY, vertexCX, vertexCY))
    {
        cout << "The vertices lie on one line, it is not a triangle." << endl;
    }
    else
    {
        triangleArea = calculateArea(vertexAX, vertexAY, vertexBX, vertexBY,
                                     vertexCX, vertexCY);
        areasSum = calculateArea(pointX, pointY, vertexAX, vertexAY, vertexBX,
                                 vertexBY)
                   + calculateArea(pointX, pointY, vertexBX, vertexBY, vertexCX,
                                   vertexCY)
                   + calculateArea(pointX, pointY, vertexCX, vertexCY, vertexAX,
                                   vertexAY);

        if (fabs(areasSum - triangleArea) <= ACCURACY * triangleArea)
        {
            cout << "The point belongs to the triangle." << endl;
        }
        else
        {
            cout << "The point does not belong to the triangle." << endl;
        }
    }
}

/*
 * Функція обробляє одну сторону багатокутника: перевіряє, чи лежить
 * на ній точка, і чи перетинає її промінь, що йде від точки праворуч.
 * Нижній кінець сторони рахується, верхній ні, тому вершина на рівні
 * променя не дає зайвого перетину
 * Параметри: fromX, fromY, toX, toY - кінці сторони
 * Значення, що повертаються: немає
 */
void checkSide(float fromX, float fromY, float toX, float toY)
{
    if (isOnOneLine(fromX, fromY, toX, toY, pointX, pointY)
        && (pointX - fromX) * (pointX - toX) <= 0
        && (pointY - fromY) * (pointY - toY) <= 0)
    {
        isOnSide = true;
    }

    if ((fromY <= pointY && toY > pointY) || (toY <= pointY && fromY > pointY))
    {
        crossingX = fromX + (pointY - fromY) * (toX - fromX) / (toY - fromY);

        if (crossingX > pointX)
        {
            crossingCount++;
        }
    }
}

/*
 * Функція вводить вершини багатокутника по одній і відразу обробляє
 * кожну сторону. Пам'ятає лише першу й попередню вершини. Непарна
 * кількість перетинів променя означає, що точка всередині
 * Параметри: немає
 * Значення, що повертаються: немає
 */
void checkPolygon()
{
    cout << endl << "Polygon" << endl;
    cout << "Enter the number of vertices n (at least 3): ";
    cin >> vertexCount;
    while (cin.fail() || vertexCount < MIN_VERTICES)
    {
        cin.clear();            // скинути помилку введення
        cin.ignore(1000, '\n'); // і пропустити неправильний рядок
        cout << "Error! Enter an integer n, at least 3: ";
        cin >> vertexCount;
    }

    cout << "Enter x and y of vertex 1: ";
    startX = inputCoordinate();
    startY = inputCoordinate();
    previousX = startX;
    previousY = startY;

    for (i = 2; i <= vertexCount; i++)
    {
        cout << "Enter x and y of vertex " << i << ": ";
        currentX = inputCoordinate();
        currentY = inputCoordinate();
        checkSide(previousX, previousY, currentX, currentY);
        previousX = currentX;
        previousY = currentY;
    }

    checkSide(previousX, previousY, startX, startY); // остання сторона

    if (isOnSide)
    {
        cout << "The point lies on a side, so it belongs to the polygon."
             << endl;
    }
    else if (crossingCount % 2 == 1)
    {
        cout << "The point is inside the polygon." << endl;
    }
    else
    {
        cout << "The point is outside the polygon." << endl;
    }
}

int main()
{
    welcome();

    cout << "Enter x and y of the point: ";
    pointX = inputCoordinate();
    pointY = inputCoordinate();
    cout << endl;

    checkTriangle();
    checkPolygon();
    return 0;
}
