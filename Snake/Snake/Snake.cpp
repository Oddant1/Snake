// Snake.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"  // cout
#include "windows.h" // console, color, and coord
#include "time.h"    // time
#include "conio.h"   // _kbhit, _getch

#define SCORE_LEN 7
#define DEFAULT 7
#define GREEN 2
#define BLUE 1
#define RED 4
#define CONSOLAS 9

using namespace std;

// Function to disable cursor
void showConsoleCursor(bool showFlag);

// Function to print colored text
char printColor(int color = DEFAULT, char text = NULL);

// Basic game operation functions
void setUp();
void input();
void logic();

// Prints to specified screen coordinates
void goToXY(int, int, char, int);
void goToXY(int, int, int);

// Get the console and the coordinates of the cursor
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorPosition;

// Initializes board
int height = 21;
int width = 21;
char wall = '#';

// Initializes snake
char snakeHead = 'O';
char snakeBody = 'o';
int maxLength = 100;
int snakeLength = 5;
int currentLength = 0;
int snakeX = (width / 2) + 1;
int snakeY = (height / 2) + 1;
int tail = 0;
int head = 0;
int prevHead = 0;
COORD *snake;
enum eDirection {STOP, LEFT, RIGHT, UP, DOWN};
eDirection direction = STOP;
eDirection previousDirection = STOP;

// Initializes apple
char apple = '+';
int appleX;
int appleY;
void spawnApple();

int score = 0;

bool wrap = true;
bool gameOver = false;

int main()
{
    showConsoleCursor(false);     // Disable cursor

	CONSOLE_FONT_INFOEX font;

	GetCurrentConsoleFontEx(console, false, &font);
	font.nFont = CONSOLAS;
	SetCurrentConsoleFontEx(console, false, &font);

    srand((int)time(NULL));                   

    setUp();                                  
    
    while (!gameOver)            // While the game is not over
    {
        input();                              
        if (direction != STOP)
        {
            logic();
            Sleep(75);           // Wait so game doesn't run too fast
        } 
    }

    delete snake;
    system("cls");               // Clear the screen when the game is over
    cout << "Game Over: ";                    

    return 0;
}

void setUp()
{
    snake = new COORD[maxLength];

    // Set up board
    for (int x = 0; x <= width + 1; x++)
    {
        goToXY(x, 0, wall, BLUE);
        goToXY(x, height + 1 , wall, BLUE);
    }
    cout << endl << "score: ";
    for (int y = 0; y <= height; y++)
    {
        goToXY(0, y, wall, BLUE);
        goToXY(width + 1, y, wall, BLUE);
    }

    // Spawn snake
    goToXY(snakeX, snakeY, snakeHead, GREEN);
    snake[head].X = snakeX;
    snake[head].Y = snakeY;
    currentLength++;

    // Spawn initial apple
    spawnApple();

    // Set up scoreboard
    goToXY(SCORE_LEN, height + 2, score);
}

void input()
{
    // If key button hit get input
    if (_kbhit())
    {
        switch (_getch()) 
        {
            case 'w' :
                if (previousDirection == DOWN)
                {
                    break;
                }
                direction = UP;
                previousDirection = direction;
                break;
            case 's' :
                if (previousDirection == UP)
                {
                    break;
                }
                direction = DOWN;
                previousDirection = direction;
                break;
            case 'a' :
                if (previousDirection == RIGHT)
                {
                    break;
                }
                direction = LEFT;
                previousDirection = direction;
                break;
             case 'd' :
                 if (previousDirection == LEFT)
                 {
                     break;
                 }
                direction = RIGHT;
                previousDirection = direction;
                break;
             case 'x' : gameOver = true;
                break;
        }
    }
}

void logic()
{
    // Move snake based on input
    switch (direction)
    {
        case UP: snakeY--;
            break;
        case DOWN: snakeY++;
            break;
        case LEFT: snakeX--;
            break;
        case RIGHT: snakeX++;
            break;
        default:
            break;
    }
    if (wrap == false)
    {
        // End game if snake hits board edge
        if (snakeX == 0 || snakeX == width + 1 || snakeY == 0 || snakeY == height + 1)
        {
            gameOver = true;
            return;
        }
    }
    else
    {
        // Wrap if snake hits board edge
        if (snakeX == 0)
        {
            snakeX = width;
        }
        else if (snakeX == width + 1)
        {
            snakeX = 1;
        }
        else if (snakeY == 0)
        {
            snakeY = height;
        }
        else if (snakeY == height + 1)
        {
            snakeY = 1;
        }
    }
    // Get rid of extra snake piece
    if (currentLength >= snakeLength)
    {
        goToXY(snake[tail].X, snake[tail].Y, ' ', DEFAULT);

        currentLength--;
    }

    // Add current snake piece to array
    prevHead = head;
    head++;
    head = head % maxLength;

    tail = head - currentLength;
    if (tail < 0)
    {
        tail += maxLength;
    }

    snake[head].X = snakeX;
    snake[head].Y = snakeY;
    goToXY(snakeX, snakeY, snakeHead, GREEN);                       // Move snake head
    goToXY(snake[prevHead].X, snake[prevHead].Y, snakeBody, GREEN); // Turn old head into body
    

    // Increment score and length then spawn new apple if snake eats apple
    if (snakeX == appleX && snakeY == appleY)
    {
        if (snakeLength < maxLength)
        {
            snakeLength++;
        }
        
        score++;
        goToXY(SCORE_LEN, height + 2, score);
        spawnApple();
    }
    
    // End game if snake hits itself
    for (int i = tail; i < tail + currentLength - 1; i++)
    {
        if (snakeX == snake[i % maxLength].X && snakeY == snake[i % maxLength].Y)
        {
            gameOver = true;
            return;
        }
    }
    
    currentLength++;
}

void spawnApple()
{
    // Spawn apple at random location within board
    appleX = (rand() % width) + 1;
    appleY = (rand() % height) + 1;

    // Make sure the apple does not spawn on the snake
    for (int i = tail; i < tail + currentLength - 1; i++)
    {
        if (appleX == snake[i % maxLength].X && appleY == snake[i % maxLength].Y)
        {
            appleX = (rand() % width) + 1;
            appleY = (rand() % height) + 1;
            i = 1;
        }
    }

    goToXY(appleX, appleY, apple, RED); // Spawn the apple
}

// Set cursor position then set string at position
void goToXY(int x, int y, char c, int color = DEFAULT)
{
    cursorPosition.X = x;
    cursorPosition.Y = y;
    SetConsoleCursorPosition(console, cursorPosition);
    cout << printColor(color, c);
    printColor();
}
void goToXY(int x, int y, int i)
{
    cursorPosition.X = x;
    cursorPosition.Y = y;
    SetConsoleCursorPosition(console, cursorPosition);
    cout << i;
}

void showConsoleCursor(bool showFlag)
{
    CONSOLE_CURSOR_INFO cursorInfo;               

    GetConsoleCursorInfo(console, &cursorInfo);       // Get cursor

    // set the cursor visibility
    cursorInfo.bVisible = showFlag;               
    SetConsoleCursorInfo(console, &cursorInfo);         
}

char printColor(int color, char text)
{
    SetConsoleTextAttribute(console, color);
    return text;
}
