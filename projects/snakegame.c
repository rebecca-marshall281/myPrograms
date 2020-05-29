#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

bool gameOver;
const int width = 30;
const int height = 15;
int x, y, foodX, foodY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection {
    STOP = 0, LEFT, RIGHT, UP, DOWN,
};
enum eDirection dir;

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;    // will center snake at beginning of game
    y = height / 2;
    time_t t;
    srand((unsigned) time(&t));   // initializes random number generator
    foodX = rand() % (width-4);
    foodY = rand() % (height-4);
    score = 0;
}

void Draw(){
    system("cls");
    for (int i = 0; i < width+2; i++){   //print top of map
        printf("#");
    }
    printf("\n");

    for(int j = 0; j < height; j++){      // print sides of map
        for(int i = 0; i < width; i++){
            if (i==0){
                printf("#");
            }
            if (i == x && j == y){
                printf("O");
            }
            else if (i == foodX && j == foodY) {
                printf("F");
            }
            else {
                bool print = false;
                for(int k = 0; k < nTail; k++)
                {
                    if(tailX[k] == i && tailY[k] == j){
                        printf("o");
                        print = true;
                    }

                }
                if (!print){
                    printf(" ");
                }

            }
        }
        printf("#\n");     //print bottom
    }

    for (int i = 0; i < width+2; i++){
        printf("#");
    }
    printf("\n");
    printf("Score:%d\n", score);
    //printf("%d %d", foodX, foodY);
}




void Input(){

    if(_kbhit()){          // function checks if user presses keyboard
        switch(_getch()){  // each case below defines the direction of snake
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }
}

void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for(int i = 1; i < nTail; i++ )   // this will allow the tail to follow the head
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch(dir)        // during each state the snake will move accordingly (up, down, left or right)
    {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }
    if (x > width || x < 0 || y > height || y < 0){  // will exit game if head goes outside map
        gameOver = true;
    }
    for (int i = 0; i < nTail; i++)    // checks if head hits tail and terminates game
    {
        if (tailX[i] == x && tailY[i] == y){
            gameOver = true;
        }

    }
    if (x == foodX && y == foodY ){
        score += 10;                        // increases score if head "eats" tail
        foodX = rand() % width;             // places food at new spot
        foodY = rand() % height;
        nTail ++;                           // adds a new tail
    }



}
int main() {

    Setup();
    while(!gameOver){
        Draw();
        Input();
        Logic();
        Sleep(100);
    }

    return 0;
}