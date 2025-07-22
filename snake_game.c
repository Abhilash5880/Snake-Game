#include <stdio.h>
#include <stdlib.h>
#include <conio.h>   // For _kbhit() and _getch()
#include <windows.h> // For Sleep()

// Game board dimensions
#define WIDTH 20
#define HEIGHT 20

// Snake and food characters
#define SNAKE_HEAD 'O'
#define SNAKE_BODY 'o'
#define FOOD '@'

// Directions
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

// Global variables
int gameOver;
int score;
int headX, headY;       // Snake head coordinates
int foodX, foodY;       // Food coordinates
int tailX[100], tailY[100]; // Snake body coordinates
int nTail;              // Length of the snake's tail
enum Direction dir;

// Function to set up the game
void Setup() {
    gameOver = 0;
    dir = STOP;
    headX = WIDTH / 2;
    headY = HEIGHT / 2;
    // Ensure food doesn't spawn on the head initially
    do {
        foodX = rand() % WIDTH;
        foodY = rand() % HEIGHT;
    } while (foodX == headX && foodY == headY);
    score = 0;
    nTail = 0;
}

// Function to draw the game board
void Draw() {
    system("cls"); // Clear console for Windows

    // Top border
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");

    // Game area
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0)
                printf("#"); // Left wall

            if (i == headY && j == headX)
                printf("%c", SNAKE_HEAD); // Snake head
            else if (i == foodY && j == foodX)
                printf("%c", FOOD); // Food
            else {
                int printTail = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("%c", SNAKE_BODY); // Snake body
                        printTail = 1;
                        break; // Found part of tail, no need to check further
                    }
                }
                if (!printTail)
                    printf(" "); // Empty space
            }

            if (j == WIDTH - 1)
                printf("#"); // Right wall
        }
        printf("\n");
    }

    // Bottom border
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");

    printf("Score: %d\n", score);
    printf("Controls: W (Up), A (Left), S (Down), D (Right), X (Exit)\n");
}

// Function to handle user input
void Input() {
    if (_kbhit()) { // Check if a key is pressed
        switch (_getch()) { // Get the pressed key
            case 'a':
            case 'A':
                // Prevent immediate reverse direction
                if (dir != RIGHT) dir = LEFT;
                break;
            case 'd':
            case 'D':
                if (dir != LEFT) dir = RIGHT;
                break;
            case 'w':
            case 'W':
                if (dir != DOWN) dir = UP;
                break;
            case 's':
            case 'S':
                if (dir != UP) dir = DOWN;
                break;
            case 'x':
            case 'X':
                gameOver = 1; // Exit game
                break;
        }
    }
}

// Function to update game logic
void Logic() {
    // Shift tail segments
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = headX; // The first tail segment takes the head's previous position
    tailY[0] = headY;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Move head
    switch (dir) {
        case LEFT:
            headX--;
            break;
        case RIGHT:
            headX++;
            break;
        case UP:
            headY--;
            break;
        case DOWN:
            headY++;
            break;
        case STOP:
            // Do nothing if game is stopped (e.g., at the start)
            break;
    }

    // Check for wall collision
    if (headX < 0 || headX >= WIDTH || headY < 0 || headY >= HEIGHT)
        gameOver = 1;

    // Check for self-collision
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == headX && tailY[i] == headY) {
            gameOver = 1;
            break; // No need to check further if collision detected
        }
    }

    // Check for food collision
    if (headX == foodX && headY == foodY) {
        score += 10;
        // Spawn new food, ensure it's not on the snake
        do {
            foodX = rand() % WIDTH;
            foodY = rand() % HEIGHT;
        } while (
            (foodX == headX && foodY == headY) || // Not on head
            (
                ({ // Check if new food is on any tail segment
                    int onTail = 0;
                    for (int i = 0; i < nTail; i++) {
                        if (tailX[i] == foodX && tailY[i] == foodY) {
                            onTail = 1;
                            break;
                        }
                    }
                    onTail;
                })
            )
        );
        nTail++; // Increase snake length
    }
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100); // Control game speed (100 milliseconds)
    }
    system("cls"); // Clear screen one last time
    printf("===================================\n");
    printf("        GAME OVER! 🐍\n");
    printf("        Final Score: %d\n", score);
    printf("===================================\n");
    return 0;
}